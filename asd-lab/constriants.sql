CREATE DATABASE college;

USE college;

CREATE TABLE subjects (
    sub_id INT NOT NULL,
    sub_name varchar(40) NOT NULL
);

INSERT INTO subjects
VALUES
    (1, "Maths"),
    (2, "Physics"),
    (3, "Chemistry"),
    (4, "English")
;

INSERT INTO subjects
VALUES (5, NULL);

SELECT * FROM subjects;

ALTER TABLE subjects
ADD CONSTRAINT pk_constraint PRIMARY KEY(sub_id);

DESCRIBE subjects;


CREATE TABLE staff (
    staff_id INT UNIQUE,
    staff_name VARCHAR(40),
    dept VARCHAR(40),
    age INT,
    salary DECIMAL(6, 0),
    CONSTRAINT age_constraint CHECK (age > 22),
    CONSTRAINT salary_constraint CHECK (salary < 35000)
);

INSERT INTO staff
VALUES
    (1, "John", "Purchasing", 24, 30000),
    (2, "Sera", "Sales", 25, 20000),
    (3, "Jane", "Sales", 28, 25000)
;

INSERT INTO staff
VALUES (1, "Johnas", "Purchasing", 26, 30000);

INSERT INTO staff
VALUES (4, "Johnas", "Purchasing", 20, 30000);

INSERT INTO staff
VALUES (4, "Johnas", "Purchasing", 20, 40000);


ALTER TABLE staff
DROP CONSTRAINT salary_constraint;

ALTER TABLE staff
DROP INDEX staff_id;

CREATE TABLE bank (
    bank_code VARCHAR(3),
    bank_name VARCHAR(40),
    head_office VARCHAR(40),
    branches INT
);

ALTER TABLE bank
ADD CONSTRAINT pk_constraint PRIMARY KEY (bank_code),
ADD CONSTRAINT branches_constraint CHECK (branches > 0),
MODIFY COLUMN bank_name VARCHAR(40) NOT NULL;

INSERT INTO bank 
VALUES 
    ("AAA", "SIB", "Ernakulam", 6),
    ("BBB", "Federal", "Kottayam", 5),
    ("CCC", "Canara", "Trivandrum", 3),
    ("SBT", "Indian", "Delhi", 7)
;


INSERT INTO bank 
VALUES ("AAA", "abcd", "Ernakulam", 4);

INSERT INTO bank 
VALUES ("DDD", NULL, "Ernakulam", 4);

INSERT INTO bank 
VALUES ("AAA", "abcd", "Ernakulam", -1);

CREATE TABLE branch (
    branch_id INT,
    branch_name VARCHAR(40) DEFAULT "New Delhi",
    bank_id VARCHAR(3),
    PRIMARY KEY (branch_id),
    CONSTRAINT fk_constraint FOREIGN KEY (bank_id) REFERENCES bank(bank_code) 
        ON UPDATE CASCADE 
        ON DELETE CASCADE
);

INSERT INTO branch
VALUES 
    (1, "Kottayam", "CCC"),
    (5, "Calicut", "SBT")
;

INSERT INTO branch (branch_id, bank_id)
VALUES (1, "CCC");

INSERT INTO branch (branch_id, bank_id)
VALUES (4, "DDD");

INSERT INTO branch (branch_id, bank_id)
VALUES (4, "AAA");

SELECT * FROM branch;

DELETE FROM bank
WHERE bank_code = "SBT";

SELECT * FROM bank;

SELECT * FROM branch;

ALTER TABLE branch
DROP PRIMARY KEY;

DESCRIBE branch;

CREATE VIEW sales_staff AS
SELECT * FROM staff
WHERE dept = "Sales";

DESCRIBE sales_staff;

SELECT * FROM sales_staff;

DROP TABLE branch;

CREATE TABLE branch(
    branch_id INT,
    branch_name VARCHAR(40) DEFAULT ("New Delhi"),
    bank_id VARCHAR(3),
    CONSTRAINT pk PRIMARY KEY(branch_id),
    CONSTRAINT fk FOREIGN KEY (bank_id) REFERENCES bank(bank_code) 
        ON UPDATE CASCADE 
        ON DELETE CASCADE
);

DESCRIBE branch;

ALTER TABLE branch
ALTER branch_name DROP DEFAULT;

DESCRIBE branch;


ALTER TABLE branch
DROP PRIMARY KEY;

DESCRIBE branch;


CREATE OR REPLACE VIEW sales_staff AS
SELECT *
FROM staff
WHERE dept = "Sales" AND salary > 20000; 

SELECT * FROM sales_staff;

DROP VIEW sales_staff;

SELECT * from sales_staff;





