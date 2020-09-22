CREATE or REPLACE FUNCTION prime(n INT) RETURNS VOID as
$$
DECLARE
    count INT =0;
    isPrime INT;
    i INT;
    currentNumber INT=2;
    rem INT;
BEGIN
    WHILE (count < n) LOOP
        isPrime = 1;
        FOR i IN 2..(currentNumber/2) LOOP
            rem = currentNumber % i;
            IF rem = 0 THEN
                isPrime = 0;
            END IF;
        END LOOP;
        IF isPrime = 1 THEN
            RAISE NOTICE '%' , currentNumber;
            count = count + 1;
        END IF;
        currentNumber = currentNumber + 1;
    END LOOP;
END;
$$ LANGUAGE plpgsql;

SELECT FROM prime(6);

-----------------------------------------------------------------

CREATE or REPLACE FUNCTION fib(n INT) RETURNS VOID as
$$
DECLARE
    first INT = 1;
    second INT = 1;
    third INT;
    count INT;
BEGIN
    RAISE NOTICE '%', first;
    RAISE NOTICE '%', second;
    FOR count in 3..n LOOP
        third = first + second;
        RAISE NOTICE '%', third;
        first = second;
        second = third;
    END LOOP;
END;
$$ LANGUAGE plpgsql;

SELECT * FROM fib(7);

--------------------------------------------------------------------

CREATE TABLE student_grade (
    roll_no INT PRIMARY KEY,
    name VARCHAR(40),
    mark1 INT,
    mark2 INT,
    mark3 INT,
    grade VARCHAR(10)
);

CREATE or REPLACE FUNCTION insert_student(rollNO INT, name VARCHAR(40), mark1 INT, mark2 INT, mark3 INT) RETURNS VOID as
$$
DECLARE
    grade VARCHAR(10);
BEGIN
    IF (mark1 + mark2 + mark3)/3 > 40 THEN
        grade = 'pass';
    ELSE
        grade = 'fail';
    END IF;
    INSERT INTO student_grade
    VALUES (rollNO, name, mark1, mark2, mark3, grade); 
END;
$$ LANGUAGE plpgsql;

SELECT FROM insert_student(1, 'Abcd', 40, 50, 45);
SELECT FROM insert_student(2, 'Ccde', 35, 35, 40);
SELECT FROM insert_student(3, 'Cdef', 40, 55, 45);

SELECT * FROM student_grade;

--------------------------------------------------------------------

CREATE or REPLACE FUNCTION create_circle() RETURNS VOID as
$$
DECLARE
    radius INT = 5;
    area DECIMAL(10, 2);
    n INT = 5;
    i INT ;
BEGIN
    CREATE TABLE circle(radius INT, area DECIMAL(10, 2));
    FOR i IN 1..n LOOP
        area = 3.14 * radius * radius;
        INSERT INTO circle VALUES(radius, area);
        radius = radius + 5;
    END LOOP;
END;
$$ LANGUAGE plpgsql;

SELECT FROM create_circle();
SELECT * FROM circle;

-------------------------------------------------------------------

CREATE TABLE student(
    name VARCHAR(10),
    mark INT
);

CREATE or REPLACE FUNCTION insert_stud() RETURNS VOID as
$$
DECLARE
    marks INT []= '{ 25, 76, 43, 45, 67, 57, 97, 56, 89, 8 }';
    names VARCHAR(20)[] = '{ ABCD, EFGH, IJKL, MNOP, QRST, UVWX, YZAB, MARK, CDEF, GHIJ }';
    i INT ;
BEGIN
    FOR i IN 1..10 LOOP
        INSERT INTO student VALUES(names[i],marks[i]);
    END LOOP;
END;
$$ LANGUAGE plpgsql;

SELECT FROM insert_stud();

SELECT * FROM student;

------------------------------------------------------------------

CREATE TABLE students (
    roll_no INT PRIMARY KEY,
    name VARCHAR(40),
    phone VARCHAR(11)
);

CREATE SEQUENCE table_key
START 1
INCREMENT BY 1;

CREATE or REPLACE FUNCTION generate_table() RETURNS VOID as
$$
DECLARE
    names VARCHAR(20)[] = '{ ABCD, EFGH, IJKL, MNOP, QRST }';
    phone VARCHAR []= '{ 9874563210, 7896541230, 8795641230, 1236547890, 8529631470 }';
    i INT ;
BEGIN
    FOR i IN 1..5 LOOP
        INSERT INTO students VALUES(NEXTVAL('table_key'), names[i], phone[i]);
    END LOOP;
END;
$$ LANGUAGE plpgsql;

SELECT FROM generate_table();

SELECT * FROM students;