CREATE TABLE customer (
    cust_id INT PRIMARY KEY,
    meter_type INT,
    previous_reading INT,
    current_reading INT,
    customer_type CHAR(2),
    bill_amount DECIMAL(10, 2),
    last_bill_payment CHAR(2)
);

INSERT INTO customer(cust_id, meter_type, previous_reading, current_reading, customer_type, last_bill_payment) 
VALUES 
    (10005, 1, 3500, 3550, 'R', 'N'),
    (10004, 3, 4800, 4989, 'C', 'Y'),
    (10003, 1, 5600, 5700, 'I', 'Y'),
    (10008, 3, 5700, 6000, 'A', 'N'),
    (10002, 3, 4890, 5042, 'C', 'Y')
;

CREATE TABLE bill (
    cust_id INT,
    bill_date DATE,
    fixed_charge DECIMAL(10, 2),
    energy_charge DECIMAL(10, 2),
    sur_charge DECIMAL(10, 2),
    net_amount DECIMAL(10, 2)
);

INSERT INTO bill
VALUES
    (10005, '24/09/2020', 40, 82, 4.1, 126.1),
    (10005, '24/07/2020', 40, 70, 3.5, 113.5),
    (10008, '24/09/2020', 120, 187.5, 18.75, 326.25)
;




    CREATE or REPLACE FUNCTION calculate_energy_charge() RETURNS VOID as
    $$
    DECLARE
        c_cust_list CURSOR FOR    
            SELECT cust_id, previous_reading, current_reading, customer_type FROM customer;
        c_cust_id customer.cust_id%type;
        c_previous_reading customer.previous_reading%type;
        c_current_reading customer.current_reading%type;
        c_customer_type customer.customer_type%type;
        units INT;
        energy_charge DECIMAL(10, 2);
    BEGIN
        OPEN c_cust_list;
        LOOP
        FETCH c_cust_list INTO c_cust_id, c_previous_reading, c_current_reading, c_customer_type;
            EXIT WHEN NOT FOUND;
                units = c_current_reading - c_previous_reading;
                IF units > 200 THEN
                    IF c_customer_type = 'A' THEN
                        energy_charge = 1.75 * units;
                    ELSIF c_customer_type = 'I' THEN
                        energy_charge = 1.5 * units;
                    ELSIF c_customer_type = 'C' THEN
                        energy_charge = 2 * units;
                    ELSIF c_customer_type = 'R' THEN
                        energy_charge = 1.6 * units;
                    END IF;
                ELSIF units > 100 THEN
                    IF c_customer_type = 'A' THEN
                        energy_charge = 1 * 100 + (units - 100) * 1.25;
                    ELSIF c_customer_type = 'I' THEN
                        energy_charge = 1.25 * 100 + (units - 100) * 1.3;
                    ELSIF c_customer_type = 'C' THEN
                        energy_charge = 1.5 * 100 + (units - 100) * 1.6;
                    ELSIF c_customer_type = 'R' THEN
                        energy_charge = 1.3 * 100 + (units - 100) * 1.4;
                    END IF;
                ELSE
                    IF c_customer_type = 'A' THEN
                        energy_charge = 1 * units;
                    ELSIF c_customer_type = 'I' THEN
                        energy_charge = 1.25 * units;
                    ELSIF c_customer_type = 'C' THEN
                        energy_charge = 1.5 * units;
                    ELSIF c_customer_type = 'R' THEN
                        energy_charge = 1.3 * units;
                    END IF;
                END IF;

                INSERT INTO bill(cust_id, energy_charge)
                VALUES (c_cust_id, energy_charge);
        END LOOP;
    END;
    $$ LANGUAGE plpgsql;

    SELECT FROM calculate_energy_charge();

---------------------------------------------------------------------------


CREATE or REPLACE FUNCTION calculate_net_amount() RETURNS VOID as
$$
DECLARE
    c_cust_list CURSOR FOR    
        SELECT c.cust_id, energy_charge, bill_date, meter_type FROM customer AS c, bill AS b WHERE c.cust_id = b.cust_id;
    c_cust_id customer.cust_id%type;
    c_energy_charge bill.energy_charge%type;
    c_bill_date bill.bill_date%type;
    c_meter_type customer.meter_type%type;
    c_fixed_charge DECIMAL(10, 2);
    c_sur_charge DECIMAL(10, 2);
    net_charge DECIMAL(10, 2);
BEGIN
    OPEN c_cust_list;
    LOOP
    FETCH c_cust_list INTO c_cust_id, c_energy_charge, c_bill_date, c_meter_type;
        EXIT WHEN NOT FOUND;
            IF c_bill_date IS NULL THEN
                IF c_meter_type = 1 THEN
                    c_fixed_charge = 40 + 17;
                    c_sur_charge = 0.05 * c_energy_charge;
                ELSE
                    c_fixed_charge = 120 + 17;
                    c_sur_charge = 0.1 * c_energy_charge;
                END IF;
                net_charge = c_energy_charge + c_fixed_charge + c_sur_charge;
                
                UPDATE bill
                SET net_amount = net_charge, bill_date = CURRENT_DATE, fixed_charge = c_fixed_charge, sur_charge = c_sur_charge
                WHERE cust_id = c_cust_id AND bill_date IS NULL;
            END IF;
    END LOOP;
END;
$$ LANGUAGE plpgsql;

SELECT FROM calculate_net_amount();

SELECT * FROM bill;

---------------------------------------------------------------------------

CREATE OR REPLACE PROCEDURE updateBillAmount(
  id int
)
AS
$$
DECLARE
  amount float;
BEGIN
  SELECT COALESCE(SUM(net_amount),0) INTO amount FROM bill WHERE cust_id=id;
  
  UPDATE customer
  SET bill_amount = amount
  WHERE cust_id = id;

END;
$$
LANGUAGE PLPGSQL;

CREATE or REPLACE FUNCTION update_bill_amount() RETURNS VOID 
AS $$
DECLARE
    c_cust_list CURSOR FOR    
        SELECT * FROM customer;
    c_cust_id customer.cust_id%type;
BEGIN
    OPEN c_cust_list;
    LOOP
    FETCH c_cust_list INTO c_cust_id;
        EXIT WHEN NOT FOUND;
            CALL updateBillAmount(c_cust_id);
    END LOOP;
END;
$$
LANGUAGE plpgsql;

SELECT FROM update_bill_amount();


--------------------------------------------------------------
CREATE OR REPLACE FUNCTION show_dues() RETURNS TRIGGER 
AS
$$
BEGIN
	IF (OLD.last_bill_payment = 'N') THEN
    raise info 'Previous dues for customer is %', OLD.bill_amount;
    END IF;

    RETURN NEW;
END;
$$
LANGUAGE PLPGSQL;


CREATE TRIGGER dues_trigger
AFTER UPDATE OF bill_amount ON customer
FOR EACH ROW
EXECUTE PROCEDURE show_dues();

----------------------------------------------------










