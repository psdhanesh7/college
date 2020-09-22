CREATE TABLE student (
    id INT PRIMARY KEY,
    name VARCHAR(40),
    m1 INT,
    m2 INT,
    m3 INT,
    grade CHAR(2)
);

INSERT INTO student(id, name, m1, m2, m3)
VALUES
    (10, 'jan', 58, 61, 29),
    (30, 'karuna', 87, 79, 77),
    (88, 'anu', 39, 67, 92),
    (29, 'jossy', 39, 80, 45)
;

SELECT * FROM student;

CREATE OR REPLACE FUNCTION calculate_grade() RETURNS VOID as
$$
DECLARE
    c_students CURSOR FOR
        SELECT * FROM student;
    c_id student.id%type;
    c_name student.name%type;
    c_m1 student.m1%type;
    c_m2 student.m2%type;
    c_m3 student.m3%type;
    total INT;
    c_grade CHAR(2);
BEGIN
    OPEN c_students;
    LOOP
    FETCH c_students INTO c_id, c_name, c_m1, c_m2, c_m3;
        EXIT WHEN NOT FOUND;
        total = c_m1 + c_m2 + c_m3;
        IF total >= 270 THEN
            c_grade = 'O';
        ELSIF total >= 255 THEN
            c_grade = 'A+';
        ELSIF total >= 240 THEN
            c_grade = 'A';
        ELSIF total >= 210 THEN
            c_grade = 'B+';
        ELSIF total >= 180 THEN
            c_grade = 'B';
        ELSIF total >= 150 THEN
            c_grade = 'C';
        ELSIF total >= 135 THEN
            c_grade = 'P';
        ELSE
            c_grade = 'F';
        END IF;
        UPDATE student
        SET grade = c_grade
        WHERE id = c_id;
    END LOOP;
END
$$ LANGUAGE plpgsql;

SELECT FROM calculate_grade();

SELECT * FROM student;

----------------------------------------------------


CREATE TABLE bank_details (
    accno INT PRIMARY KEY,
    name VARCHAR(40),
    balance INT,
    adate DATE
);

INSERT INTO bank_details
VALUES
    (1001, 'aby', 3005, '2015-10-10'),
    (1005, 'majo', 6600, '2001-01-01'),
    (1002, 'alan', 4000, '1995-05-05'),
    (1003, 'amal', 5000, '1992-03-16'),
    (1004, 'jeffin', 3500, '2050-04-01')
;

SELECT * FROM bank_details;

CREATE TABLE interest (
    accno INT,
    interest DECIMAL(7, 2)
);

CREATE OR REPLACE FUNCTION calculate_interest() RETURNS VOID AS
$$
DECLARE
    c_bank_details CURSOR FOR
        SELECT accno, balance FROM bank_details;
    c_accno bank_details.accno%type;
    c_balance bank_details.balance%type;
    c_interest interest.interest%type;
BEGIN
    OPEN c_bank_details;
    LOOP
    FETCH c_bank_details INTO c_accno, c_balance;
        EXIT WHEN NOT FOUND;
        c_interest = 0.08 * c_balance;
        INSERT INTO interest
        VALUES(c_accno, c_interest);
    END LOOP;
END
$$LANGUAGE plpgsql;

SELECT FROM calculate_interest();

SELECT * FROM interest;

-------------------------------------------------------------------


CREATE TABLE people_list (
    id INT,
    name VARCHAR(40),
    dt_joining DATE,
    place VARCHAR(40)
);

INSERT INTO people_list
VALUES
    (101, 'Robert', '2005-04-03', 'CHY'),
    (102, 'Mathew', '2008-06-07', 'CHY'),
    (103, 'Luffy', '2003-04-15', 'FSN'),
    (104, 'Lucci', '2009-08-13', 'KTM'),
    (105, 'Law', '2005-04-14', 'WTC'),
    (101, 'Vivi', '2010-09-21', 'ABA')
;

SELECT * FROM people_list;

CREATE TABLE experience_list (
    id INT,
    name VARCHAR(40),
    experience INT
);

CREATE OR REPLACE FUNCTION calculate_experience() RETURNS VOID AS
$$
DECLARE
    c_people_list CURSOR FOR
        SELECT id, name, dt_joining FROM people_list;
    c_id people_list.id%type;
    c_name people_list.name%type;
    c_dt_joining people_list.dt_joining%type;
    c_experience INT;
BEGIN
    OPEN c_people_list;
    LOOP
    FETCH c_people_list INTO c_id, c_name, c_dt_joining;
        EXIT WHEN NOT FOUND;
        c_experience = DATE_PART('year', CURRENT_DATE::date) - DATE_PART('year', c_dt_joining::date);
        IF c_experience > 10 THEN
            INSERT INTO experience_list
            VALUES (c_id, c_name, c_experience);
        END IF;
    END LOOP;
END
$$LANGUAGE plpgsql; 

SELECT FROM calculate_experience();

SELECT * FROM experience_list;

----------------------------------------------------------------------------

CREATE TABLE employee (
    id INT PRIMARY KEY,
    name VARCHAR(40) NOT NULL,
    m_sal DECIMAL(6, 0)
);

INSERT INTO employee
VALUES
    (101, 'Mathew', 55000),
    (102, 'Jose', 80000),
    (103, 'John', 250000),
    (104, 'Ann', 600000)
;

SELECT * FROM employee;

CREATE OR REPLACE FUNCTION update_salary() RETURNS VOID AS
$$
DECLARE
    c_employee CURSOR FOR
        SELECT id, m_sal FROM employee;
    c_id employee.id%type;
    c_m_sal employee.m_sal%type;
    new_salary employee.m_sal%type;
BEGIN
    OPEN c_employee;
    LOOP
    FETCH c_employee INTO c_id, c_m_sal;
        EXIT WHEN NOT FOUND;
        IF 12 * c_m_sal < 60000 THEN
            new_salary = c_m_sal * 1.25;
        ELSIF 12 * c_m_sal < 200000 THEN
            new_salary = c_m_sal * 1.20;
        ELSIF 12 * c_m_sal < 500000 THEN
            new_salary = c_m_sal * 1.15;
        ELSE
            new_salary = c_m_sal * 1.1;
        END IF;
        UPDATE employee
        SET m_sal = new_salary
        WHERE id = c_id;
    END LOOP;
END
$$LANGUAGE plpgsql;

SELECT FROM update_salary();

SELECT * FROM employee;


