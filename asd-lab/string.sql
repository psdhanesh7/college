
CREATE DATABaSE bank;

USE bank;

CREATE TABLE acct_details (
    acct_no VARCHAR(10) PRIMARY KEY,
    branch VARCHAR(40) NOT NULL,
    name VARCHAR(40),
    phone VARCHAR(15)
);

INSERT INTO acct_details
VALUES
    ("A40123401", "Chicago", "Mike Adams", "(378)400-1234"),
    ("A40123402", "Miami", "Diana George", "(372)420-2345"),
    ("A40123403", "Miami", "Diaz Elizabeth", "(371)450-3456"),
    ("A40123404", "Atlanta", "Jeoffrey George", "(370)460-4567"),
    ("A40123405", "New York", "Jennifer Kaitlyn", "(373)470-5678"),
    ("A40123406", "Chicago", "Kaitlyn Vincent", "(318)200-3235"),
    ("A40123407", "Miami", "Abraham Gottfield", "(328)300-2256"),
    ("A50123408", "New Jersy", "Stacy Williams", "(338)400-5337"),
    ("A50123409", "New York", "Catherine George", "(348)500-6228"),
    ("A501234010", "Miami", "Oliver Scott", "(358)600-7230")
;

SELECT * FROM acct_details;

SELECT name
FROM acct_details
WHERE name LIKE "D%";

SELECT DISTINCT branch
FROM acct_details
WHERE branch LIKE "%New%";

SELECT UPPER(name) AS name
FROM acct_details;

SELECT name
FROM acct_details
WHERE name LIKE "___n%n";

SELECT name
FROM acct_details
WHERE name LIKE "D_a%" AND name LIKE "%ELi%";

SELECT name
FROM acct_details
WHERE acct_no LIKE "%6";

UPDATE acct_details
SET name = UPPER(name);

SELECT * 
FROM acct_details;

SELECT name
FROM acct_details
WHERE name LIKE "%t";

SELECT REVERSE(name)
FROM acct_details;

SELECT CONCAT("+1", phone) AS phone
FROM acct_details;

SELECT SUBSTR(acct_no, 2) AS acct_no
FROM acct_details;

SELECT *
FROM acct_details
WHERE acct_no LIKE "_4%" AND name LIKE "%Williams%";

---------------------------------------------------------------------

SELECT TRIM(LEADING "122" FROM "12341")
FROM DUAL;

