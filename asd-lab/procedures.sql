CREATE OR REPLACE FUNCTION factorial(n INT) RETURNS INT AS
$$
DECLARE
    i INT = 1;
    factorial INT = 1;
BEGIN
    FOR i IN 1..n LOOP
        factorial = factorial * i;
    END LOOP;
    RETURN factorial;
END;
$$ LANGUAGE plpgsql;

SELECT * FROM factorial(5);

-------------------------------------------------------------

CREATE TABLE student_details (
    roll INT PRIMARY KEY,
    marks INT,
    phone INT
);

INSERT INTO student_details
VALUES
    (1, 90, 1234569870),
    (2, 80, 1234569870),
    (3, 70, 1234569870),
    (4, 95, 1234569870),
    (5, 94, 1234569870)
;

SELECT * FROM student_details;

CREATE OR REPLACE PROCEDURE boost()
LANGUAGE plpgsql AS 
$$
BEGIN
    UPDATE student_details SET marks = marks * 1.05;
END;
$$;

CALL boost();


---------------------------------------------------------

CREATE TABLE student (
    id INT PRIMARY KEY,
    name VARCHAR(40),
    m1 INT,
    m2 INT,
    m3 INT,
    total INT,
    grade CHAR
);

INSERT INTO student (id, name, m1, m2, m3)
VALUES
    (1, 'abcd', 90, 90, 90),
    (2, 'cdef', 80, 85, 70),
    (3, 'ghij', 89, 78, 90),
    (4, 'klmno', 87, 76, 96),
    (5, 'pqrs', 90, 90, 84)
;

SELECT * FROM student;

CREATE OR REPLACE FUNCTION calculate_grade(total INT) RETURNS CHAR AS
$$
DECLARE
    grade student.grade%type;
BEGIN
    IF total >= 240 THEN
        grade = 'A';
    ELSIF total >= 180 THEN
        grade = 'B';
    ELSIF total >= 120 THEN
        grade = 'C';
    ELSIF total >= 60 THEN
        grade = 'D' ;
    ELSE
        grade = 'F';
    END IF;
    RETURN grade;
END;
$$
LANGUAGE plpgsql;

CREATE OR REPLACE PROCEDURE update_grade()
LANGUAGE plpgsql
AS $$
DECLARE
    c_students CURSOR FOR
        SELECT * FROM student;
    c_id student.id%type;
    c_name student.name%type;
    c_m1 student.m1%type;
    c_m2 student.m2%type;
    c_m3 student.m3%type;
    grade student.grade%type;
    c_total INT;
BEGIN
    OPEN c_students;
    LOOP
    FETCH c_students INTO c_id, c_name, c_m1, c_m2, c_m3;
        EXIT WHEN NOT FOUND;
        c_total = c_m1 + c_m2 + c_m3;
        UPDATE student
        SET grade = calculate_grade(c_total), total = c_total
        WHERE id = c_id;
    END LOOP;
END;
$$;

CALL update_grade();

SELECT * FROM student;

--------------------------------------------------------------------


CREATE SCHEMA pk1;

CREATE OR REPLACE PROCEDURE pk1.proc1(num1 INT, num2 INT)
LANGUAGE plpgsql
AS
$$
DECLARE
    sum INT;
    average REAL;
    product INT;
BEGIN
    sum = num1 + num2;
    product = num1 * num2;
    average = (num1 + num2)/2;
    RAISE NOTICE 'Sum of % and % is %', num1, num2, sum;
    RAISE NOTICE 'Product of % and % is %', num1, num2, product;
    RAISE NOTICE 'Average of % and % is %', num1, num2, average;
END;
$$;

CREATE OR REPLACE PROCEDURE pk1.proc2(num1 INT)
LANGUAGE plpgsql
AS
$$
DECLARE
    root INT;
BEGIN
    root = sqrt(num1);
    RAISE NOTICE 'Root of % is %', num1, root;
END;
$$;

CREATE OR REPLACE FUNCTION pk1.fn11(num INT) RETURNS VOID AS
$$
DECLARE
    isOdd INT ;
BEGIN
    isOdd = num % 2;
    IF isOdd = 1 THEN
        RAISE NOTICE 'Number % is odd', num;
    ELSE
        RAISE NOTICE 'Number % is even', num;
    END IF;
END;
$$ LANGUAGE plpgsql;

CREATE OR REPLACE FUNCTION pk1.fn22(num1 INT, num2 INT, num3 INT) RETURNS VOID AS
$$
DECLARE
    sum INT ;
BEGIN
    sum = num1 + num2 + num3;
    RAISE NOTICE 'Sum of %, %, % is %', num1, num2, num3, sum;
END;
$$ LANGUAGE plpgsql;

CREATE OR REPLACE FUNCTION pk1.callAllFunctions(num1 INT,num2 INT, num3 INT)
RETURNS VOID AS
$$
DECLARE
BEGIN
    CALL pk1.proc1(num1, num2);
    CALL pk1.proc2(num1);
    PERFORM pk1.fn11(num1);
    PERFORM pk1.fn22(num1, num2, num3);
END;
$$ LANGUAGE plpgsql;

SELECT FROM pk1.callAllFunctions(25, 35, 45);
