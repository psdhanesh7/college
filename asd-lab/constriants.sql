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

ALTER TABLE subjects
ADD CONSTRAINT pk_constraint PRIMARY KEY(sub_id);

ALTER TABLE subjects
DROP CONSTRAINT pk_constraint; 

DESCRIBE subjects;

SELECT * FROM subjects;

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

INSERT INTO bank (bank_code, bank_name, head_office, branches)
VALUES ("DDD", "abcd", "Ernakulam", -1);

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
VALUES
    (3, "AAA"),
    (4, "BBB")
;

DELETE FROM bank
WHERE bank_code = "SBT";

ALTER TABLE branch
DROP PRIMARY KEY;

CREATE VIEW sales_staff AS
SELECT * FROM staff
WHERE dept = "Sales";

DESCRIBE sales_staff;

SELECT * FROM sales_staff;

DROP TABLE branch;

CREATE TABLE branch(
    branch_id INT,
    branch_name VARCHAR(40) CONSTRAINT df DEFAULT ("New Delhi"),
    bank_id VARCHAR(3),
    CONSTRAINT pk PRIMARY KEY(bank_id),
    CONSTRAINT fk FOREIGN KEY (bank_id) REFERENCES bank(bank_code) 
        ON UPDATE CASCADE 
        ON DELETE CASCADE
);
