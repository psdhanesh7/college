CREATE DATABASE students;

USE students;

CREATE TABLE student (
    roll_no INT PRIMARY KEY,
    name VARCHAR(20) NOT NULL,
    Physics INT,
    Chemistry INT,
    Maths INT
);

INSERT INTO student
VALUES
    (1,"Adam",20,20,33),
    (2,"Bob",18,9,41),
    (3,"Bright",22,7,31),
    (4,"Duke",13,21,20),
    (5,"Elvin",14,22,23),
    (6,"Fetcher",2,10,48),
    (7,"Georgina",22,12,22),
    (8,"Mary",24,14,31),
    (9,"Tom",19,15,24),
    (10,"Zack",8,20,36)
;

SELECT AVG(Physics) FROM student;

SELECT MAX(Maths) AS highest_marks_maths
FROM student;

SELECT MIN(Chemistry) AS lowest_mark_chemistry
FROM student;

SELECT COUNT(*)
FROM student
WHERE Physics >= 12;


SELECT *
FROM student
WHERE Physics >= 12 AND Chemistry >= 12 AND Maths >= 25;

SELECT RANK() 
OVER (ORDER BY (Physics + Chemistry + Maths) DESC) 
rank_no, roll_no, name, Physics, Chemistry, Maths,
IF((Physics >= 12 AND Chemistry >= 12 AND Maths >= 25),"PASS","FAIL") AS result
FROM student;

SELECT (COUNT(*) * 100/(SELECT COUNT(*) FROM student))
AS maths_pass_percent
FROM student
WHERE Maths>=25;

SELECT (COUNT(*) * 100 / (SELECT COUNT(*) FROM student))
AS pass_percentage
FROM student
WHERE Physics >= 12 AND Chemistry >= 12 AND Maths >= 25;

SELECT AVG(Physics + Chemistry + Maths) AS average
FROM student;

SELECT COUNT(*)
FROM student
WHERE Physics >= 12 AND Chemistry >= 12 AND Maths >= 25;
