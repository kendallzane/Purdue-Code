set termout off

-- Admin Tables

create table Users(
	UserId integer,
	Username varchar(32),
	Password varchar(32),
	constraint user_unique unique (Username),
	primary key (UserId));

create table Roles(
	RoleId integer,
	RoleName varchar(32),
	EncryptionKey varchar(32),
	constraint role_unique unique (RoleName),
	primary key (RoleId));

create table UsersRoles(
	UserId integer,
	RoleId integer,
	primary key (userId, RoleId),
	foreign key (UserId) references Users(UserId),
	foreign key (RoleId) references Roles(RoleId));

create table Privileges(
	PrivId integer,
	PrivName varchar(32),
	constraint priv_unique unique (PrivName),
	primary key (PrivId));

create table RolesPrivileges(
	RoleId integer,
	PrivId integer,
	TableName varchar(32),
	primary key(RoleId, PrivId, TableName),
	foreign key (RoleId) references Roles(RoleId),
	foreign key (PrivId) references Privileges(PrivId));

----------------------------------------------------------------------------------------------

-- User Tables

create table Customers(
	CustomerId integer,
	FirstName varchar(30),
	LastName varchar(30),
	Address varchar(200),
	PhoneNo varchar(10),
	Age integer,
	EncryptedColumn number(10,0),
	OwnerRole number(10, 0),
	primary key(CustomerId));

create table Suppliers(
	SupplierId integer,
	FirstName varchar(30),
	LastName varchar(30),
	Address varchar(200),
	PhoneNo varchar(10),
	EncryptedColumn number(10,0),
	OwnerRole number(10, 0),
	primary key(SupplierId));

create table Products(
	ProductId integer,
	ProductName varchar(30),
	Category varchar(30),
	SupplierId integer,
	ProductDescription varchar(100),
	EncryptedColumn number(10,0),
	OwnerRole number(10, 0),
	primary key(ProductId));

create table Inventory(
	ProductId integer,
	TotalStock integer,
	LastUpdatedOn date,
	EncryptedColumn number(10,0),
	OwnerRole number(10, 0),
	primary key(ProductId));

set termout on
