set serveroutput on size 32000

CREATE OR REPLACE VIEW o AS SELECT Orders.OrderId, Orders.CustomerId, Product.Category, Customer.Age, OrderItems.Quantity, 10*(CEIL(Customer.Age/10)) AS RoundedAge
FROM Orders, OrderItems, Product, Customer
WHERE Orders.OrderId = OrderItems.OrderId and OrderItems.ProductId = Product.ProductId and Orders.CustomerId = Customer.CustomerId
GROUP BY Orders.OrderId, Orders.CustomerId, Product.Category, Customer.Age, OrderItems.Quantity;



create or replace procedure Pro_order_status as
num_delayed_month number;
num_delayed_year number;
num_shipped_month number;
num_shipped_year number;
CURSOR order_cur is SELECT OrderId, OrderDate, Status FROM Orders;
order_rec order_cur%ROWTYPE;
begin
	num_delayed_month := 0;
	num_delayed_year := 0;
	num_shipped_month := 0;
	num_shipped_year := 0;
	for order_rec in order_cur loop
	--DBMS_OUTPUT.PUT_LINE(MONTHS_BETWEEN(CURRENT_DATE, order_rec.OrderDate));
		if (MONTHS_BETWEEN(CURRENT_DATE, order_rec.OrderDate)) < 1.00 then
			if order_rec.Status = 'shipped' then
				num_shipped_month := num_shipped_month + 1;
			else
				num_delayed_month := num_delayed_month + 1;
			end if;
		elsif (MONTHS_BETWEEN(CURRENT_DATE, order_rec.OrderDate)) < 12.00 then
			if order_rec.Status = 'shipped' then
				num_shipped_year := num_shipped_year + 1;
			else
				num_delayed_year := num_delayed_year + 1;
			end if;
		end if;
	end loop;
DBMS_OUTPUT.PUT_LINE('DURATION	DELAYED_ORDERS	SHIPPED_ORDERS');
DBMS_OUTPUT.PUT_LINE('--------------------------------------------------------------');
DBMS_OUTPUT.PUT_LINE('LAST_MONTH	'||num_delayed_month||'		'||num_shipped_month);
DBMS_OUTPUT.PUT_LINE('LAST_YEAR	'||num_delayed_year||'		'||num_shipped_year);
end Pro_order_status;
/
begin
Pro_order_status;
end;
/

create or replace procedure Pro_prod_report as
num_available number;
num_critical number;
num_oos number;
CURSOR inv_cur is SELECT ProductId, TotalStock FROM Inventory;
p_name Product.ProductName%TYPE;
p_sid Product.SupplierId%TYPE;
p_firstname Supplier.FirstName%TYPE;
p_lastname Supplier.LastName%TYPE;
p_phone Supplier.PhoneNo%TYPE;
inv_rec inv_cur%ROWTYPE;
CURSOR name(pid IN Product.ProductId%TYPE) is SELECT ProductName FROM Product where ProductId=pid ORDER BY ProductName DESC;
begin
num_available := 0;
num_critical := 0;
num_oos := 0;
for inv_rec in inv_cur loop
	if (inv_rec.TotalStock > 10) then
		num_available := num_available + 1;
	elsif (inv_rec.TotalStock <= 0) then
		num_oos := num_oos + 1;
	else 
		num_critical := num_critical + 1;
	end if;
end loop;

DBMS_OUTPUT.PUT_LINE('INVENTORY REPORT');
DBMS_OUTPUT.PUT_LINE('AVAILABLE	CRITICAL	OUT OF STOCK');
DBMS_OUTPUT.PUT_LINE('--------------------------------------------------------------');
DBMS_OUTPUT.PUT_LINE(num_available||'		'||num_critical||'		'||num_oos);
DBMS_OUTPUT.PUT_LINE(chr(10));
DBMS_OUTPUT.PUT_LINE('Out of stock products');
DBMS_OUTPUT.PUT_LINE('Product id	Product Name	Supplier Name		Supplier Phone No.');
DBMS_OUTPUT.PUT_LINE('-------------------------------------------------------------------------');

for inv_rec in inv_cur loop
	if (inv_rec.TotalStock <= 0)then
		--select ProductName into p_name from Product where inv_rec.ProductId = p_id;
		select ProductName into p_name from Product where ProductId = inv_rec.ProductId;
		select SupplierId into p_sid from Product where ProductId = inv_rec.ProductId;
		select FirstName into p_firstname from Supplier where SupplierId = p_sid;
		select LastName into p_lastname from Supplier where SupplierId = p_sid;
		select PhoneNo into p_phone from Supplier where SupplierId = p_sid;
		DBMS_OUTPUT.PUT_LINE(inv_rec.ProductId||'		'||p_name||'		'||p_firstname||' '||p_lastname||'	'||p_phone);
		
	end if;
end loop;

end Pro_prod_report;
/
begin
Pro_prod_report;
end;
/



create or replace procedure Pro_age_categ as
last_age number := -1;
temp number := 0;
CURSOR ov is SELECT SUM(Quantity) AS numTotal, Category, RoundedAge FROM o GROUP BY RoundedAge, Category ORDER BY RoundedAge ASC, numTotal DESC, Category DESC;
item_order ov%ROWTYPE;
begin
DBMS_OUTPUT.PUT_LINE('HIT CATEGORIES'||chr(10));
for item_order in ov loop
	if last_age = -1 then
		last_age := item_order.RoundedAge - 10;
	end if;
	loop
		if item_order.RoundedAge - last_age > 10 then
			temp := last_age;
			last_age := last_age + 10;
			DBMS_OUTPUT.PUT_LINE('>'||temp||', <='||last_age);
			DBMS_OUTPUT.PUT_LINE('-------------------------');
			DBMS_OUTPUT.PUT_LINE('NA'||chr(10));
		end if;
		exit when item_order.RoundedAge - last_age <= 10;
	end loop;
	if item_order.RoundedAge - last_age = 10 then
		DBMS_OUTPUT.PUT_LINE('>'||last_age||', <='||item_order.RoundedAge);
		DBMS_OUTPUT.PUT_LINE('-------------------------');
		DBMS_OUTPUT.PUT_LINE(item_order.Category||chr(10));
	elsif item_order.RoundedAge - last_age = 0 then
	last_age := last_age;
	end if;
	last_age := item_order.RoundedAge;
end loop;
end Pro_age_categ;
/
begin
Pro_age_categ;
end;
/

CREATE OR REPLACE VIEW p AS SELECT Product.ProductId, Product.Category, OrderItems.Quantity, OrderItems.UnitPrice, OrderItems.Discount,
SUM(OrderItems.Quantity * OrderItems.UnitPrice) AS mPrice, SUM(OrderItems.Quantity * OrderItems.Discount) AS mDiscount
FROM OrderItems, Product
WHERE OrderItems.ProductId = Product.ProductId
GROUP BY Product.ProductId, Product.Category, OrderItems.Quantity, OrderItems.UnitPrice, OrderItems.Discount;

create or replace procedure Pro_category_info as
CURSOR op is SELECT ProductId, Category, Quantity, UnitPrice, Discount, mPrice, mDiscount FROM p GROUP BY ProductId, Category, Quantity, UnitPrice, Discount, mPrice, mDiscount ORDER BY Category DESC;
item_info ov%ROWTYPE;

CURSOR report is SELECT Category, SUM(Quantity) AS TotalUnits, SUM(mPrice)/SUM(QUANTITY) AS AVG_price, SUM(mDiscount)/SUM(QUANTITY) AS AVG_discount FROM p GROUP BY Category ORDER BY TotalUnits DESC;
report_info report%ROWTYPE;
begin 

DBMS_OUTPUT.PUT_LINE('CATEGORY REPORT');
DBMS_OUTPUT.PUT_LINE('CATEGORY		TOTAL_UNITS	AVG_PRICE	AVG_DISCOUNT');
DBMS_OUTPUT.PUT_LINE('-----------------------------------------------------------------------');
for report_info in report loop
	DBMS_OUTPUT.PUT_LINE(RPAD(report_info.Category, 16, ' ')||'	'||report_info.TotalUnits||'	'||TO_CHAR(report_info.AVG_price, '99999999999.99')||'		'||TO_CHAR(report_info.AVG_discount, '99999999999.99'));
end loop;
end Pro_category_info;
/
begin
Pro_category_info;
end;
/

--according to piazza post @274 I am allowed to have the output for this problem in this format

CREATE OR REPLACE VIEW customerOrders AS SELECT Customer.CustomerId, Customer.FirstName, Customer.LastName, Customer.Age, Customer.PhoneNo, Orders.OrderId
FROM Customer, Orders
WHERE Customer.CustomerId = Orders.CustomerId;

create or replace procedure Pro_search_customer(inputId IN Customer.CustomerId%TYPE) as
CURSOR cust_cur is SELECT COUNT(OrderId) AS NumOrders, CustomerId, FirstName, LastName, PhoneNo, Age FROM customerOrders
WHERE CustomerId = inputId
GROUP BY CustomerId, FirstName, LastName, PhoneNo, Age;
customer_info cust_cur%ROWTYPE;
exist number := 0;
begin
for customer_info in cust_cur loop
	exist := 1;
	DBMS_OUTPUT.PUT_LINE('Customer Name : '||customer_info.FirstName||' '||customer_info.LastName);
	DBMS_OUTPUT.PUT_LINE('Customer Age : '||customer_info.Age);
	DBMS_OUTPUT.PUT_LINE('Phone No : '||customer_info.PhoneNo);
	DBMS_OUTPUT.PUT_LINE('No. of order placed : '||customer_info.NumOrders);
end loop;
	if exist < 1 then
		DBMS_OUTPUT.PUT_LINE('Customer with id '||inputId||' not found');
	end if;
end Pro_search_customer;

/
begin
Pro_search_customer(1);
end;
/

CREATE OR REPLACE VIEW supp AS SELECT Supplier.SupplierId, Supplier.FirstName, Supplier.LastName, Supplier.PhoneNo, Supplier.Address, Product.ProductId
FROM Supplier, Product
WHERE Supplier.SupplierId = Product.SupplierId;

create or replace procedure Pro_search_supplier(inputId IN Supplier.SupplierId%TYPE) as
CURSOR supp_cur is SELECT COUNT(ProductId) AS NumProducts, SupplierId, FirstName, LastName, PhoneNo, Address FROM supp
WHERE SupplierId = inputId
GROUP BY SupplierId, FirstName, LastName, PhoneNo, Address;
supp_info supp_cur%ROWTYPE;
exist number := 0;

CURSOR product_search is SELECT ProductId, ProductName, SupplierId FROM Product;
product_info product_search%ROWTYPE;

begin
for supp_info in supp_cur loop
	exist := 1;
	DBMS_OUTPUT.PUT_LINE('Supplier Name : '||supp_info.FirstName||' '||supp_info.LastName);
	DBMS_OUTPUT.PUT_LINE('Supplier Phone No : '||supp_info.PhoneNo);
	DBMS_OUTPUT.PUT_LINE('Supplier Address : '||supp_info.Address);
	DBMS_OUTPUT.PUT_LINE('No. of products supplied : '||supp_info.NumProducts);
end loop;
	if exist < 1 then
		DBMS_OUTPUT.PUT_LINE('Supplier with id '||inputId||' not found');
	else
		DBMS_OUTPUT.PUT_LINE('Products :');
		DBMS_OUTPUT.PUT_LINE('Product Id	Product Name');
		DBMS_OUTPUT.PUT_LINE('----------------------------------------');
		for product_info in product_search loop
			if (product_info.SupplierId = inputId) then
				DBMS_OUTPUT.PUT_LINE(product_info.ProductId||'		'||product_info.ProductName);
			end if;
		end loop;
	end if;
end Pro_search_supplier;
/
begin
Pro_search_supplier(3);
end;
/