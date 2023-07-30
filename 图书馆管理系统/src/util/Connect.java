package util;

/**
 * 数据库工具类
 */
import java.sql.Connection;
import java.sql.DriverManager;

public class Connect {
	Connection con;
	/**
	 * 连接数据库
	 * @return
	 */
	public Connection loding() {
		try {
			Class.forName("com.mysql.cj.jdbc.Driver");
			System.out.println("成功加载数据库驱动！");
		}
		catch(Exception e){
			System.out.println("加载数据库驱动出错！");
			e.printStackTrace();
		}

		try {
			con = DriverManager
					.getConnection(
							"jdbc:mysql://localhost:3306/book_admin?useSSL=false&allowPublicKeyRetrieval=true&serverTimezone=UTC","root","Wu123456");
			System.out.println("成功连接数据库服务器");
		}
		catch(Exception e1){
			System.out.println("连接数据库服务器出现错误");
		}
		
		return con;
	}
	/**
	 * 关闭数据库
	 * @param con
	 * @throws Exception
	 */
	public void closeCon (java.sql.Connection con)throws Exception {
        if(con!=null){
            con.close();
        }
    }
}