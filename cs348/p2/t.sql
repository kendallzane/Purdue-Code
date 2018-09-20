create table Customer(CustomerId integer, FirstName varchar(30), LastName varchar(30), Address varchar(200), PhoneNo varchar(10), Age integer, primary key(CustomerId));
create table Supplier(SupplierId integer, FirstName varchar(30), LastName varchar(30), Address varchar(200), PhoneNo varchar(10), primary key(SupplierId));
create table Product(ProductId integer, ProductName varchar(30), Category varchar(30), SupplierId integer, ProductDescription varchar(100), primary key(ProductId));
create table Inventory(ProductId integer, TotalStock integer, LastUpdatedOn date);
create table Orders(OrderId integer, CustomerId integer, OrderDate date, Status varchar(20),  primary key(OrderId));
create table OrderItems(OrderId integer, ProductId integer, Quantity integer, UnitPrice Number(10,4), Discount Number(4,2));
