//STEP 1. Import required packages
import java.sql.*;
import java.io.*;

public class Project3 {
	static final String JDBC_DRIVER = "oracle.jdbc.OracleDriver";  
	static final String DB_URL = "jdbc:oracle:thin:@claros.cs.purdue.edu:1524:strep";
	
	static final String USER = "bowlesk";
	static final String PASS = "ktNAfbOn";
	   
	@SuppressWarnings("unused")
	public static void main(String[] args) {
	
	//System.out.println("Program start");
	 
	String inText = args[0];
	String outText = args[1];
	 
	Connection conn = null;
	Statement stmt = null;
	String line = null;
	String delims = "[ ]+";
	String sql;
	
	String currentUsername = null;
	int currentUserId = -1;
	boolean userIsAdmin = false;
	
	
	
		try {
			//STEP 2: Register JDBC driver
			Class.forName(JDBC_DRIVER);
			//STEP 3: Open a connection
			//System.out.println("Connecting to database...");
			conn = DriverManager.getConnection(DB_URL,USER,PASS);
			//System.out.println("Connected");
			
			FileReader fileReader = new FileReader(inText);
			BufferedReader bufferedReader = new BufferedReader(fileReader);
			
			FileWriter fw = new FileWriter(outText);
			PrintWriter pw = new PrintWriter(fw);
			int commandNum = 0;
			boolean quit = false;
			while((line = bufferedReader.readLine()) != null && quit == false) {
				commandNum++;
				pw.println(commandNum + ": " + line);
	            //System.out.println(line);
	            String[] tokens = line.split(delims);
	            
	            //System.out.println("command type = " + tokens[0]);
	            
	            if ((tokens[0]).equals("LOGIN")) {
	            	//System.out.println("attempting login");
	            	stmt = conn.createStatement();
	            	sql = "SELECT * FROM Users " +
	            		"WHERE Username='" + tokens[1] 
	            		+ "' AND Password='" + tokens[2] + "'";
	            	//System.out.println("sql command : " + sql);
	            	ResultSet rs = stmt.executeQuery(sql);
	            	boolean found = false;
	            	int id = 0;
	            	while(rs.next()){
	                    String un = rs.getString("Username");
	                    String pass = rs.getString("Password");
	                    id = rs.getInt("UserId");
	                    //System.out.println("Found Username: " + un);
	                    //System.out.println("Found Password: " + pass);
	                    //System.out.println("Found UserId: " + id);
	                    found = true;
	                 }
	            	if (found) {
	            		//System.out.println("Login successful, updating current user");
	            		currentUsername = tokens[1];
	            		currentUserId = id;
	            		if ((currentUsername).equals("admin")) {
	            			//System.out.println("Setting current user to admin mode");
	            			userIsAdmin = true;
	            		} else {
	            			//System.out.println("Setting current user to NOT admin mode");
	            			userIsAdmin = false;
	            		}
	            		pw.println("Login successful");
	            	} else {
	            		//System.out.println("Login not successful, user not found");
	            		pw.println("Invalid login");
	            	}
	            	pw.println();
	            }
	            if ((tokens[0]).equals("CREATE")) {
	            	if (userIsAdmin == false) {
	            		//System.out.println("Can't create, user not admin");
	            		//System.out.println();
	            		pw.println("Authorization failure");
	            	} else {
	            		if ((tokens[1]).equals("ROLE")) {
	            			//System.out.println("Attempting to create a role");
	            			//System.out.println("Getting id to use");
	            			stmt = conn.createStatement();
	    	            	sql = "SELECT MAX(RoleId) AS MaxId FROM Roles";
	    	            	ResultSet rs = stmt.executeQuery(sql);
	    	            	int id = 0;
	    	            	while(rs.next()){
	    	                    id = rs.getInt("MaxId");
	    	                    //System.out.println("Found Max RoleId: " + id);
	    	                }
	    	            	id++;
	    	            	//System.out.println("Using RoleId: " + id);
	    	            	
	    	            	//System.out.println("Adding Role");
	    	            	
	    	            	sql = "insert into Roles values(" + id + ", '" + tokens[2]
	    	            			+ "', '" + tokens[3] + "')";
	    	            	//System.out.println("sql command : " + sql);
	    	            	stmt.executeQuery(sql);
	    	            	//System.out.println("Role successfully added");
	    	            	//System.out.println();
	    	            	pw.println("Role created successfully");
	            		}
	            		if ((tokens[1]).equals("USER")) {
	            			//System.out.println("Attempting to create a user");
	            			//System.out.println("Getting id to use");
	            			stmt = conn.createStatement();
	    	            	sql = "SELECT MAX(UserId) AS MaxId FROM Users";
	    	            	ResultSet rs = stmt.executeQuery(sql);
	    	            	int id = 0;
	    	            	while(rs.next()){
	    	                    id = rs.getInt("MaxId");
	    	                    //System.out.println("Found Max UserId: " + id);
	    	                }
	    	            	id++;
	    	            	//System.out.println("Using UserId: " + id);
	    	            	
	    	            	//System.out.println("Adding User");
	    	            	
	    	            	sql = "insert into Users values(" + id + ", '" + tokens[2]
	    	            			+ "', '" + tokens[3] + "')";
	    	            	//System.out.println("sql command : " + sql);
	    	            	stmt.executeQuery(sql);
	    	            	//System.out.println("User successfully added");
	    	            	//System.out.println();
	    	            	pw.println("User created successfully");
	            		}
	            	}
	            	pw.println();
	            }
	            if ((tokens[0]).equals("GRANT")) {
	            	if (userIsAdmin == false) {
	            		//System.out.println("Can't grant, user not admin");
	            		//System.out.println();
	            		pw.println("Authorization failure");
	            	} else {
	            		if ((tokens[1]).equals("ROLE")) {
	            			//System.out.println("Attempting to grant a role");
	            			int userId = 0;
	            			int roleId = 0;
	            			//System.out.println("First: get userid");
	            			sql = "SELECT UserId FROM Users WHERE Username='" + tokens[2] + "'";
	            			ResultSet rs = stmt.executeQuery(sql);
	            			while(rs.next()){
	            				userId = rs.getInt("UserId");
	    	                 }
	            			//System.out.println("userid is " + userId);
	            			//System.out.println("Second: get roleid");
	            			sql = "SELECT RoleId FROM Roles WHERE RoleName='" + tokens[3] + "'";
	            			rs = stmt.executeQuery(sql);
	            			while(rs.next()){
	            				roleId = rs.getInt("RoleId");
	    	                 }
	            			//System.out.println("roleid is " + roleId);
	            			//System.out.println("adding to UserRoles table now");
	            			sql = "insert into UsersRoles values(" + userId + ", " + roleId + ")";
	            			//System.out.println("sql command : " + sql);
	    	            	stmt.executeQuery(sql);
	    	            	//System.out.println("Role successfully granted");
	    	            	//System.out.println();
	    	            	pw.println("Role assigned successfully");
	            		}
	            		if ((tokens[1]).equals("PRIVILEGE")) {
	            			//System.out.println("Attempting to grant a privilege");
	            			int privId = 0;
	            			int roleId = 0;
	            			//System.out.println("First: get privid");
	            			sql = "SELECT PrivId FROM Privileges WHERE PrivName='" + tokens[2] + "'";
	            			ResultSet rs = stmt.executeQuery(sql);
	            			while(rs.next()){
	            				privId = rs.getInt("PrivId");
	    	                 }
	            			//System.out.println("privid is " + privId);
	            			//System.out.println("Second: get roleid");
	            			sql = "SELECT RoleId FROM Roles WHERE RoleName='" + tokens[4] + "'";
	            			rs = stmt.executeQuery(sql);
	            			while(rs.next()){
	            				roleId = rs.getInt("RoleId");
	    	                 }
	            			//System.out.println("roleid is " + roleId);
	            			//System.out.println("adding to RolesPrivileges table now");
	            			sql = "insert into RolesPrivileges values(" + roleId + ", " + privId + ", '" + tokens[6] + "')";
	            			//System.out.println("sql command : " + sql);
	    	            	stmt.executeQuery(sql);
	    	            	//System.out.println("Privilege successfully granted");
	    	            	//System.out.println();
	    	            	pw.println("Privilege granted successfully");
	            		}
	            	}
	            	pw.println();
	            }
	            if ((tokens[0]).equals("REVOKE")) {
	            	if (userIsAdmin == false) {
	            		//System.out.println("Can't revoke, user not admin");
	            		//System.out.println();
	            		pw.println("Authorization failure");
	            	} else {
	            		//System.out.println("Attempting to revoke a privilege");
	            		int privId = 0;
            			int roleId = 0;
            			//System.out.println("First: get privid");
            			sql = "SELECT PrivId FROM Privileges WHERE PrivName='" + tokens[2] + "'";
            			ResultSet rs = stmt.executeQuery(sql);
            			while(rs.next()){
            				privId = rs.getInt("PrivId");
    	                 }
            			//System.out.println("privid is " + privId);
            			//System.out.println("Second: get roleid");
            			sql = "SELECT RoleId FROM Roles WHERE RoleName='" + tokens[4] + "'";
            			rs = stmt.executeQuery(sql);
            			while(rs.next()){
            				roleId = rs.getInt("RoleId");
    	                 }
            			//System.out.println("roleid is " + roleId);
            			//System.out.println("removing from RolesPrivileges table now");
            			sql = "DELETE FROM RolesPrivileges WHERE RoleId=" + roleId + " AND PrivId=" + privId 
            					+ " AND TableName='" + tokens[6] + "'";
            			//System.out.println("sql command : " + sql);
    	            	stmt.executeQuery(sql);
    	            	//System.out.println("Privilege successfully revoked");
    	            	//System.out.println();
    	            	pw.println("Privilege revoked successfully");
	            	}
	            	pw.println();
	            }
	            if ((tokens[0]).equals("INSERT")) {
	            	//System.out.println("Attempting to insert into a table");
	            	//System.out.println("First, check permissions");
	            	boolean allowed = false;
	            	sql = "SELECT UsersRoles.RoleId, RolesPrivileges.PrivId, RolesPrivileges.TableName FROM UsersRoles JOIN RolesPrivileges ON UsersRoles.RoleId=RolesPrivileges.RoleId" +
	            			//"INNER JOIN RolesPrivileges, UserRoles " +
	            			//"ON UsersRoles.RoleId=RolesPrivileges.RoleId";
	            			" WHERE UsersRoles.UserId=" + currentUserId + " AND RolesPrivileges.TableName='" + tokens[2] + "'"; 
	            	//System.out.println("sql command : " + sql);
	            	ResultSet rs = stmt.executeQuery(sql);
	            	int roleId = 0;
	            	while(rs.next()){
        				int privId = rs.getInt("PrivId");
        				roleId = rs.getInt("RoleId");
        				if (privId == 1) {
        					allowed = true;
        				}
	                 }
	            	if (allowed == false) {
	            		//System.out.println("Permission to insert on this table not found");
	            		//System.out.println();
	            		pw.println("Authorization failure");
	            	} else {
	            		//System.out.println("Permission to insert on this table found");
	            		//System.out.println("Attempting to add a row of elements");
	            		String inputs = "";
	            		for (int i = 3; i < tokens.length - 3; i++) {
	            			inputs = inputs + tokens[i] + " ";
	            		}
	            		//System.out.println("User table input: " + inputs);
	            		String delim = "[']";
	            		String[] inputValues = inputs.split(delim);
	            		
	            		int counter = 0;
	            		String[] realInputs = new String[(inputValues.length - 1) / 2];
	            		for (int i = 1; i < inputValues.length; i = i + 2) {
	            			realInputs[counter] = inputValues[i];
	            			//System.out.println("Element: " + inputValues[i]);
	            			counter++;
	            		}
	            		////System.out.println("ne: " + counter);
	            		String columnNo = tokens[tokens.length - 2];
	            		String ownerRole = tokens[tokens.length - 1];
	            		//System.out.println("columnNo: " + columnNo);
	            		//System.out.println("ownerRole: " + ownerRole);
	            		int columnNum = Integer.parseInt(columnNo);
	            		if (columnNum > 0) {
	            			//System.out.println("Need to encrypt a row");
	            		
	            		String toEncrypt = realInputs[columnNum - 1];
	            		//System.out.println("Encrypting " + toEncrypt + ", getting key");
	            		String EncryptionKey = "";
	            		sql = "SELECT EncryptionKey FROM Roles WHERE RoleName='" + ownerRole + "'";
	            		rs = stmt.executeQuery(sql);
	            		while(rs.next()){
	        				EncryptionKey = rs.getString("EncryptionKey").toUpperCase();
		                 }
	            		//System.out.println("Encryption Key is " + EncryptionKey);
	            		//System.out.println("Begin encryption process");
	            		String encrypted = "";
	            		for (int i = 0, j = 0; i < toEncrypt.length(); i++)
	                    {
	                        char charToChange = toEncrypt.charAt(i);
	                        boolean isLetter = false;
	                        boolean isLowerCase = false;
	                        if (Character.isLetter(charToChange)) {
	                        	isLetter = true;
	                        }
	                        if (Character.isLowerCase(charToChange)) {
	                        	isLowerCase = true;
	                        }
	                        charToChange = Character.toUpperCase(charToChange);
	                        
	                        if (!isLetter) {
	                        	encrypted += charToChange;
	                        } else {
	                        	char changedChar = (char) ((charToChange + EncryptionKey.charAt(j) - 2 * 'A') % 26 + 'A');
	                        	if (isLowerCase) {
	                        		encrypted += Character.toLowerCase(changedChar);
	                        	} else {
	                        		encrypted += changedChar;
	                        	}
	                        	j = ++j % EncryptionKey.length();
	                        }
	                        
	                    }
	            		//System.out.println("Encrypted String is " + encrypted);
	            		realInputs[columnNum - 1] = encrypted;
	            		}
	            		sql = "SELECT RoleId FROM Roles WHERE RoleName='" + ownerRole + "'";
	            		rs = stmt.executeQuery(sql);
	            		int oid = 0;
	            		while(rs.next()){
	        				oid = rs.getInt("RoleId");
		                }
	            		sql = "insert into " + tokens[2] + " values(";
	            		for (int i = 0; i < realInputs.length; i++) {
	            			sql += "'";
	            			sql += realInputs[i];
	            			sql += "', ";
	            		}
	            		sql += "'" + columnNo + "', '";
	            		sql += oid + "')";
	            		//System.out.println("sql command : " + sql);
    	            	stmt.executeQuery(sql);
    	            	//System.out.println("Successfully added to table");
	            		//System.out.println();
	            		pw.println("Row inserted successfully");
	            	}
	            	pw.println();
	            }
	            if ((tokens[0]).equals("SELECT")) {
	            	System.out.println("Attempting to select from a table");
	            	System.out.println("First, check permissions");
	            	boolean allowed = false;
	            	sql = "SELECT UsersRoles.RoleId, RolesPrivileges.PrivId, RolesPrivileges.TableName FROM UsersRoles JOIN RolesPrivileges ON UsersRoles.RoleId=RolesPrivileges.RoleId" +
	            			//"INNER JOIN RolesPrivileges, UserRoles " +
	            			//"ON UsersRoles.RoleId=RolesPrivileges.RoleId";
	            			" WHERE UsersRoles.UserId=" + currentUserId + " AND RolesPrivileges.TableName='" + tokens[3] + "'";
	            	System.out.println("sql command : " + sql);
	            	ResultSet rs = stmt.executeQuery(sql);
	            	int roleId = 0;
	            	while(rs.next()){
        				int privId = rs.getInt("PrivId");
        				roleId = rs.getInt("RoleId");
        				if (privId == 2) {
        					allowed = true;
        				}
	                 }
	            	if (allowed == false) {
	            		//System.out.println("Permission to select on this table not found");
	            		//System.out.println();
	            		pw.println("Authorization failure");
	            	} else {
	            		System.out.println("Permission to select on this table found");
	            		int columnCount = 0;
	            		sql = "SELECT COUNT(*) FROM user_tab_columns WHERE table_name='" + tokens[3].toUpperCase() + "'";
	            		rs = stmt.executeQuery(sql);
	            		while(rs.next()){
	            			columnCount = rs.getInt(1);
	            		}
	            		sql = "SELECT table_name, column_name FROM ALL_TAB_COLUMNS WHERE TABLE_NAME='" + tokens[3].toUpperCase() + "' ORDER BY COLUMN_ID";

	            		System.out.println(sql);
	            		rs = stmt.executeQuery(sql);
	            		String firstLine = "";
	            		int counter = 1;
	            		while(rs.next()){
	        				String tName = rs.getString(1);
	        				String cName = rs.getString(2);
	        				if (tName.equals(tokens[3].toUpperCase())) {
	        					
	        					if (counter <= columnCount - 2) {
	        						firstLine += cName + ", ";
	        					}
	        					counter++;
	        				}
	            		}
	            		firstLine = firstLine.substring(0, firstLine.length() - 2);
	            		System.out.println(firstLine);
	            		pw.println(firstLine);
	            		
	            		sql = "SELECT COUNT("+ tokens[3] +".*, Roles.EncryptionKey) FROM "+ tokens[3] + " LEFT JOIN UsersRoles ON UsersRoles.RoleId = " + tokens[3] + ".OwnerRole " + 
	            				" LEFT JOIN Roles ON Roles.RoleId = UsersRoles.RoleId AND" + 
	            				" UsersRoles.UserId=" + currentUserId;
	            		System.out.println(sql);
	            		//rs = stmt.executeQuery(sql);

	            		while(rs.next()){
	            			int c = rs.getInt(1);
	        				System.out.println("count = " + c);
	        				
		                 }
	            		
	            		sql = "SELECT "+ tokens[3] +".*, Roles.EncryptionKey FROM "+ tokens[3] + " LEFT JOIN UsersRoles ON UsersRoles.RoleId = " + tokens[3] + ".OwnerRole " + 
	            				" INNER JOIN Roles ON Roles.RoleId = UsersRoles.RoleId AND" + 
	            				" UsersRoles.UserId=" + currentUserId;
	            		rs = stmt.executeQuery(sql);
	            		String newLine = "";
	            		String[] lineValues = new String[columnCount - 1];
	            		String toRead = "";
	            		int ownerRole = -1;
	            		int encryptedColumn = -1;
	            		boolean found = false;
	            		while(rs.next()){
	            			System.out.println("exists");
	            			for (int i = 1; i <= columnCount; i++) {
	            				//toRead = rs.getString(i);
	            				////System.out.println(toRead);
	            				//newLine += toRead + ", ";
	            				if (i <= columnCount - 2) {
	            					lineValues[i] = rs.getString(i);
	            				}
	            				System.out.println("found element: " + rs.getString(i));
	            			}
	            			//newLine = newLine.substring(0, newLine.length() - 2);
	            			////System.out.println(newLine);
	            			if (rs.getInt("EncryptedColumn") > 0) {
	            				System.out.println("there is an encrypted column here");
	            				ownerRole = rs.getInt("OwnerRole");
	            				encryptedColumn = rs.getInt("EncryptedColumn");
	            				String ek = rs.getString("EncryptionKey");
	            				System.out.println("a");
	            				if (ek != null) {
	            					System.out.println("b");
		            				System.out.println(ek);
		            				System.out.println("owner role = " + ownerRole);
		            				System.out.println("Decrypting " + lineValues[encryptedColumn] + " using key " + ek);
		            				String toDecrypt = lineValues[encryptedColumn];
		            				String decrypted = "";
		            				for (int i = 0, j = 0; i < toDecrypt.length(); i++)
		    	                    {
		    	                        char charToChange = toDecrypt.charAt(i);
		    	                        boolean isLetter = false;
		    	                        boolean isLowerCase = false;
		    	                        if (Character.isLetter(charToChange)) {
		    	                        	isLetter = true;
		    	                        }
		    	                        if (Character.isLowerCase(charToChange)) {
		    	                        	isLowerCase = true;
		    	                        }
		    	                        charToChange = Character.toUpperCase(charToChange);
		    	                        
		    	                        if (!isLetter) {
		    	                        	decrypted += charToChange;
		    	                        } else {
		    	                        	char changedChar = (char) ((charToChange - ek.charAt(j) + 26) % 26 + 'A');
		    	                        	if (isLowerCase) {
		    	                        		decrypted += Character.toLowerCase(changedChar);
		    	                        	} else {
		    	                        		decrypted += changedChar;
		    	                        	}
		    	                        	j = ++j % ek.length();
		    	                        }
		    	                        
		    	                    }
		            				System.out.println("decrypted string is " + decrypted);
		            				lineValues[encryptedColumn] = decrypted;
	            				} else {
	            					System.out.println("c");
	            					System.out.println("no permission to decrypt");
	            				}
	            			}
	            			for (int i = 1; i < lineValues.length; i++) {
	            				pw.print(lineValues[i]);
	            				System.out.print(lineValues[i]);
	            				if (i < lineValues.length - 1) {
	            					System.out.print(", ");
	            					pw.print(", ");
	            				} else {
	            					System.out.println();
	            					pw.println();
	            				}
	            			}
	            			
	            		}
	            	}
	            	pw.println();
	            }
	            if ((tokens[0]).equals("QUIT")) {
	            	//System.out.println("Disconnecting from database...");
	    			if (stmt != null) {
	    				stmt.close();
	    			}
	    		    conn.close();
	    		    quit = true;
	    		    //System.out.println("Disconnected");
	            }
	            
	        }  
			
			
			if (quit == false) {
				
			
				
				
				
				//System.out.println("Disconnecting from database...");
				if (stmt != null) {
					stmt.close();
				}
			    conn.close();
			    //System.out.println("Disconnected");
			}
			bufferedReader.close();
			pw.close();
		} catch (ClassNotFoundException e) {
			e.printStackTrace();
		} catch (SQLException se) {
			se.printStackTrace();
		} catch (FileNotFoundException e) {
			e.printStackTrace();
		} catch (IOException e) {
			e.printStackTrace();
		}
		
	}

}
