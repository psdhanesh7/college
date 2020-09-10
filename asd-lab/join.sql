CREATE DATABASE orders;

USE orders;

CREATE TABLE items (
    item_id INT PRIMARY KEY NOT NULL, 
    item_name VARCHAR(40) NOT NULL,
    category VARCHAR(40),
    price INT,
    in_stock INT
);

INSERT INTO items
VALUES
    (5, "sony z5 premium", "electronics", 5005, 1),
    (4, "Samsung Galaxy S$", "electronics", 5005, 1),
    (3, "One Plus 7", "electronics", 6006, 2),
    (2, "Iphone X", "electronics", 7007, 6),
    (1, "Xiomi", "electronics", 1001, 6)
;

SELECT * FROM items;

CREATE TABLE customers (
    cust_id INT PRIMARY KEY NOT NULL,
    cust_name VARCHAR(40) NOT NULL,
    address VARCHAR(50),
    state VARCHAR(30)
);

INSERT INTO customers
VALUES
    (111, "elvin", "2020 jai street", "delhi"),
    (113, "soman", "puthumana", "kerala"),
    (115, "mickey", "juhu", "maharashtra"),
    (112, "patrick", "harinagar", "tamilnadu"),
    (114, "jaise", "kottarakara", "kerala")
;

SELECT * FROM customers;


CREATE TABLE orders (
    order_id INT PRIMARY KEY,
    item_id INT NOT NULL,
    quantity INT,
    order_date DATE DEFAULT (CURRENT_DATE),
    cust_id INT NOT NULL,
    CONSTRAINT fk_order_item FOREIGN KEY(item_id) REFERENCES items(item_id)
        ON DELETE CASCADE ON UPDATE CASCADE,
    CONSTRAINT fk_order_cust FOREIGN KEY(cust_id) REFERENCES customers(cust_id)
        ON DELETE CASCADE ON UPDATE CASCADE
);

INSERT INTO orders 
VALUES
    (1, 1, 2, '2014-10-11', 111),
    (2, 3, 1, '2012-01-29', 113),
    (3, 5, 1, '2013-05-13', 115),
    (4, 4, 3, '2014-12-22', 114)
;

INSERT INTO orders 
VALUES (5, 3, 2, '2014-10-11', 111);

SELECT * FROM orders;


CREATE TABLE delivery (
    delivery_id INT PRIMARY KEY,
    order_id INT NOT NULL,
    cust_id INT NOT NULL,
    CONSTRAINT fk_delivery_order FOREIGN KEY(order_id) REFERENCES orders(order_id)
        ON DELETE CASCADE ON UPDATE CASCADE,
    CONSTRAINT fk_delivery_cust FOREIGN KEY(cust_id) REFERENCES customers(cust_id)
        ON DELETE CASCADE ON UPDATE CASCADE
);


INSERT INTO delivery
VALUES
    (1001, 1, 111),
    (1002, 2, 113),
    (1003, 3, 115)
;

SELECT * FROM delivery;


SELECT DISTINCT cust_id, cust_name, address, state
FROM customers NATURAL JOIN orders;

SELECT DISTINCT cust_id, cust_name, address, state
FROM customers NATURAL JOIN delivery;

SELECT cust_name, order_date
FROM customers NATURAL JOIN orders
WHERE cust_name LIKE 'J%';

SELECT item_name, price
FROM customers NATURAL JOIN orders NATURAL JOIN items
WHERE cust_name = "Mickey";

SELECT DISTINCT cust_id, cust_name, address, state
FROM customers NATURAL JOIN orders
WHERE order_date > '2013-01-31'
    AND order_id NOT IN (SELECT order_id FROM delivery);

SELECT item_id
FROM items NATURAL JOIN orders
UNION
SELECT item_id
FROM items
WHERE item_id NOT IN (SELECT item_id
    FROM orders NATURAL JOIN delivery); 

SELECT cust_name
FROM customers NATURAL JOIN orders
WHERE cust_id IN (SELECT cust_id
    FROM delivery);

SELECT cust_name
FROM customers NATURAL JOIN orders
WHERE cust_name NOT IN (SELECT cust_name
    FROM customers NATURAL JOIN delivery);

SELECT cust_name
FROM 

SELECT COUNT(order_id), cust_id
FROM orders
GROUP BY cust_id;

SELECT cust_name 
FROM customers
WHERE cust_id IN (
    SELECT cust_id, MAX(order_count)
    FROM (SELECT cust_id, COUNT(order_id) AS order_count
        FROM orders
        GROUP BY cust_id) AS max_count
    GROUP BY cust_id;
);


SELECT cust_id, COUNT(order_id) AS order_count
FROM orders
GROUP BY cust_id;

SELECT cust_id, COUNT(order_id) AS order_count
FROM orders
GROUP BY cust_id
WHERE order_count = 2;

SELECT MAX(order_count)
FROM (SELECT cust_id, COUNT(order_id) AS order_count
    FROM orders
    GROUP BY cust_id) AS order_count;

SELECT cust_id, MAX(item_id)
FROM orders
GROUP BY cust_id;







