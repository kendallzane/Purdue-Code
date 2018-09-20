--Customer
insert into Customer(CustomerId, FirstName, LastName, Age, Address, PhoneNo) values (1, 'Cust A', 'Cust Last a', 22, 'ddress1', '765000001');
insert into Customer(CustomerId, FirstName, LastName, Age, Address, PhoneNo) values (2, 'Cust B', 'Cust Last b', 20, 'ddress2', '765000002');
insert into Customer(CustomerId, FirstName, LastName, Age, Address, PhoneNo) values (3, 'Cust C', 'Cust Last c', 25, 'ddress3', '765000003');
insert into Customer(CustomerId, FirstName, LastName, Age, Address, PhoneNo) values (4, 'Cust D', 'Cust Last d', 27, 'ddress4', '765000004');
insert into Customer(CustomerId, FirstName, LastName, Age, Address, PhoneNo) values (5, 'Cust E', 'Cust Last e', 20, 'ddress5', '765000001');
insert into Customer(CustomerId, FirstName, LastName, Age, Address, PhoneNo) values (6, 'Cust F', 'Cust Last f', 24, 'ddress6', '765000002');
insert into Customer(CustomerId, FirstName, LastName, Age, Address, PhoneNo) values (7, 'Cust G', 'Cust Last g', 66, 'ddress7', '765000002');

--Supplier
insert into Supplier(SupplierId, FirstName, LastName, Address, PhoneNo) values (1, 'Supp A', 'Supp Last a', 's_ddress1', '765000011');
insert into Supplier(SupplierId, FirstName, LastName, Address, PhoneNo) values (2, 'Supp B', 'Supp Last b', 's_ddress2', '765000012');
insert into Supplier(SupplierId, FirstName, LastName, Address, PhoneNo) values (3, 'Supp C', 'Supp Last c', 's_ddress3', '765000013');
insert into Supplier(SupplierId, FirstName, LastName, Address, PhoneNo) values (4, 'Supp D', 'Supp Last d', 's_ddress4', '765000014');
insert into Supplier(SupplierId, FirstName, LastName, Address, PhoneNo) values (5, 'Supp E', 'Supp Last e', 's_ddress5', '765000015');
insert into Supplier(SupplierId, FirstName, LastName, Address, PhoneNo) values (6, 'Supp F', 'Supp Last f', 's_ddress6', '765000016');

--Product
insert into Product(ProductId, ProductName, Category, SupplierId, ProductDescription) values (1, 'Iphone', 'electronics', 1, 'apple iphone X');
insert into Product(ProductId, ProductName, Category, SupplierId, ProductDescription) values (2, 'OnePlus 5t', 'electronics', 1, 'One plus 5t smart phone');
insert into Product(ProductId, ProductName, Category, SupplierId, ProductDescription) values (3, 'Nike sports jacket', 'apparel', 2, 'Nike sports training jacket - size L');
insert into Product(ProductId, ProductName, Category, SupplierId, ProductDescription) values (4, 'Adidas cap', 'apparel', 3, 'adidas brown cap - size M');
insert into Product(ProductId, ProductName, Category, SupplierId, ProductDescription) values (5, 'inferno', 'books', 4, 'dan brown - inferno');
insert into Product(ProductId, ProductName, Category, SupplierId, ProductDescription) values (6, 'paths of glory', 'books', 5, 'jeffery archer - paths of glory - fiction');
insert into Product(ProductId, ProductName, Category, SupplierId, ProductDescription) values (7, 'LG 72 inch LED TV', 'electronics', 3, 'LG tv - 72 inch 4k display, LED');
insert into Product(ProductId, ProductName, Category, SupplierId, ProductDescription) values (8, 'da vinci code', 'books', 4, 'dan brown - davinci');

--Inventory
insert into Inventory(ProductId,TotalStock,LastUpdatedOn) values (1,0,to_date('19900101','YYYYMMDD'));
insert into Inventory(ProductId,TotalStock,LastUpdatedOn) values (2,13,to_date('19900202','YYYYMMDD'));
insert into Inventory(ProductId,TotalStock,LastUpdatedOn) values (3,12,to_date('19910102','YYYYMMDD'));
insert into Inventory(ProductId,TotalStock,LastUpdatedOn) values (4,15,to_date('19920202','YYYYMMDD'));
insert into Inventory(ProductId,TotalStock,LastUpdatedOn) values (5,0,to_date('19900103','YYYYMMDD'));
insert into Inventory(ProductId,TotalStock,LastUpdatedOn) values (6,1,to_date('19900402','YYYYMMDD'));
insert into Inventory(ProductId,TotalStock,LastUpdatedOn) values (7,2,to_date('19900502','YYYYMMDD'));

--Orders
insert into Orders(OrderId,CustomerId,OrderDate,Status) values (1,1,to_date('20180401','YYYYMMDD'),'shipped');
insert into Orders(OrderId,CustomerId,OrderDate,Status) values (2,2,to_date('20180101','YYYYMMDD'),'delayed');
insert into Orders(OrderId,CustomerId,OrderDate,Status) values (3,3,to_date('20180401','YYYYMMDD'),'shipped');
insert into Orders(OrderId,CustomerId,OrderDate,Status) values (4,4,to_date('20180101','YYYYMMDD'),'delayed');
insert into Orders(OrderId,CustomerId,OrderDate,Status) values (5,5,to_date('20180101','YYYYMMDD'),'shipped');
insert into Orders(OrderId,CustomerId,OrderDate,Status) values (6,2,to_date('20180401','YYYYMMDD'),'delayed');
insert into Orders(OrderId,CustomerId,OrderDate,Status) values (7,1,to_date('20180101','YYYYMMDD'),'shipped');
insert into Orders(OrderId,CustomerId,OrderDate,Status) values (8,7,to_date('20180101','YYYYMMDD'),'shipped');
insert into Orders(OrderId,CustomerId,OrderDate,Status) values (9,7,to_date('20180101','YYYYMMDD'),'shipped');
insert into Orders(OrderId,CustomerId,OrderDate,Status) values (10,7,to_date('20180101','YYYYMMDD'),'shipped');

--OrderItems
insert into OrderItems(OrderId,ProductId,Quantity,UnitPrice,Discount) Values (1,1,1,10.5,15);
insert into OrderItems(OrderId,ProductId,Quantity,UnitPrice,Discount) Values (2,2,1,10.6,7.5);
insert into OrderItems(OrderId,ProductId,Quantity,UnitPrice,Discount) Values (3,8,10,11.5,10.99);
insert into OrderItems(OrderId,ProductId,Quantity,UnitPrice,Discount) Values (4,4,1,12.5,0);
insert into OrderItems(OrderId,ProductId,Quantity,UnitPrice,Discount) Values (5,5,1,13.5,5.5);
insert into OrderItems(OrderId,ProductId,Quantity,UnitPrice,Discount) Values (6,6,1,14.5,0);
insert into OrderItems(OrderId,ProductId,Quantity,UnitPrice,Discount) Values (7,2,1,10.5,0);
insert into OrderItems(OrderId,ProductId,Quantity,UnitPrice,Discount) Values (8,4,1,10.5,0);
insert into OrderItems(OrderId,ProductId,Quantity,UnitPrice,Discount) Values (9,3,99,10.5,0);
insert into OrderItems(OrderId,ProductId,Quantity,UnitPrice,Discount) Values (10,4,1,10.5,0);