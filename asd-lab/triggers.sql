CREATE TABLE customer_details(
    cust_id INT PRIMARY KEY,
    cust_name VARCHAR(20),
    address VARCHAR(20)
);

CREATE TABLE emp_details(
    empid INT PRIMARY KEY,
    empname VARCHAR(20),
    salary INT
);

CREATE TABLE cust_count(count_row INT);

---------------------------------------------------------------


CREATE OR REPLACE FUNCTION customer_insert() RETURNS TRIGGER AS
$customer_insert$
BEGIN
    RAISE NOTICE 'Customer inserted';
    RETURN NEW;
END;
$customer_insert$
LANGUAGE plpgsql;

CREATE TRIGGER customer_insert
AFTER INSERT ON customer_details
FOR EACH STATEMENT EXECUTE PROCEDURE customer_insert();

INSERT INTO customer_details
VALUES (1, 'Abcd', 'XYZ Street');



----------------------------------------------------------------

CREATE OR REPLACE FUNCTION check_salary() RETURNS TRIGGER AS
$check_salary$
BEGIN
    IF NEW.salary>20000 THEN
    RAISE NOTICE 'Employee % has salary > 20000',NEW.empname;
    END IF;
    RETURN NEW;
END;
$check_salary$
LANGUAGE plpgsql;

CREATE TRIGGER check_salary
AFTER INSERT OR UPDATE ON emp_details
FOR EACH ROW EXECUTE PROCEDURE check_salary();

INSERT INTO emp_details
VALUES (1, 'Abcd', 25000);

--------------------------------------------------------------

INSERT INTO cust_count VALUES(0);

CREATE OR REPLACE FUNCTION update_row_count() RETURNS TRIGGER AS
$update_row_count$
DECLARE
    count INT;
BEGIN
    SELECT * FROM cust_count INTO count;
    IF TG_OP = 'DELETE' THEN
        IF count != 0 THEN
            UPDATE cust_count SET count_row=count_row - 1;
        END IF;
    ELSIF TG_OP = 'INSERT' THEN
        UPDATE cust_count SET count_row=count_row + 1;
    END IF;
    RETURN NEW;
END;
$update_row_count$
LANGUAGE plpgsql;

CREATE TRIGGER update_row_count
AFTER INSERT OR DELETE ON customer_details
FOR EACH STATEMENT EXECUTE PROCEDURE update_row_count();

INSERT INTO customer_details
VALUES (2, 'Efgh', 'ABC Street');


-----------------------------------------------------------------
-- INSERT INTO emp_details
-- VALUES (1, 'Abcd', 25000);

-- INSERT INTO emp_details
-- VALUES (2, 'Efgh', 50000);

CREATE TABLE deleted(
    empid INT PRIMARY KEY,
    empname VARCHAR(20),
    salary INT
);


CREATE TABLE updated(
    empid INT PRIMARY KEY,
    empname VARCHAR(20),
    salary INT
);

CREATE OR REPLACE FUNCTION on_delete_update() RETURNS TRIGGER AS
$on_delete_update$
BEGIN
    IF TG_OP = 'DELETE' THEN
        INSERT INTO deleted 
        VALUES(OLD.empid,OLD.empname,OLD.salary);
    ELSIF TG_OP = 'UPDATE' THEN
        INSERT INTO updated 
        VALUES(OLD.empid,OLD.empname,OLD.salary);
    END IF;
    RETURN NEW;
END;
$on_delete_update$
LANGUAGE plpgsql;

CREATE TRIGGER on_delete_update
AFTER UPDATE OR DELETE ON emp_details
FOR EACH ROW EXECUTE PROCEDURE on_delete_update();

DELETE FROM emp_details
WHERE empid = 2;

UPDATE emp_details
SET empname = 'Dhanesh'
WHERE empid = 1;

SELECT * FROM deleted;

SELECT * FROM updated;
-----------------------------------------------------------------

CREATE OR REPLACE FUNCTION division(a INT,b INT) RETURNS INT as
$$
DECLARE
    result INT;
BEGIN
    IF b=0 THEN
        RAISE EXCEPTION division_by_zero;
    ELSE
        result = a/b;
        RETURN result;
    END IF;
END;
$$
LANGUAGE plpgsql;

SELECT * FROM division(20, 0);

SELECT * FROM division(20, 2);

--------------------------------------------------------

CREATE OR REPLACE FUNCTION get_salary_by_id(id INT) RETURNS INT as
$$
DECLARE
    sal INT;
BEGIN
    SELECT salary INTO sal FROM emp_details WHERE empid = id;
    IF sal IS NULL THEN
        RAISE EXCEPTION no_data_found;
    ELSE
        RETURN sal;
    END IF;
END;
$$
LANGUAGE plpgsql;

SELECT * FROM get_salary_by_id(1);

SELECT * FROM get_salary_by_id(3);

--------------------------------------------------------
CREATE TABLE ebill (
    cname VARCHAR(40),
    prevreading INT,
    currreading INT
);

CREATE OR REPLACE FUNCTION check_reading() RETURNS TRIGGER AS
$checkread$
BEGIN
    IF NEW.prevreading = NEW.currreading THEN
        RAISE EXCEPTION 'DATA ENTRY ERROR';
    ELSE
        RAISE NOTICE 'STATEMENT PROCESSED';
    END IF;
    RETURN NEW;
END;
$checkread$
LANGUAGE plpgsql;

CREATE TRIGGER check_reading
BEFORE INSERT ON ebill
FOR EACH ROW EXECUTE PROCEDURE check_reading();

INSERT INTO ebill
VALUES ('abc', 170, 170);

INSERT INTO ebill
VALUES ('def', 170, 169);
--------------------------------------------------------

