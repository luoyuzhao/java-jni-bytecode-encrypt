package helloworld;

import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.FileOutputStream;
import java.io.InputStream;
import java.util.Enumeration;
import java.util.jar.JarEntry;
import java.util.jar.JarFile;
import java.util.jar.JarOutputStream;

public class main {
	public static void main(String[] args) {
		jarEncrypt("D://test.jar","D://test-d.jar");
	}
	public static void jarEncrypt(String inputJarPath,String outputJarPath){
	    try {
	      ByteArrayOutputStream baos = new ByteArrayOutputStream();
	      byte[] buf = new byte[1024];
	      File srcFile = new File(inputJarPath);
	      File dstFile = new File(outputJarPath);
	      FileOutputStream dstFos = new FileOutputStream(dstFile);
	      JarOutputStream dstJar = new JarOutputStream(dstFos);
	      JarFile srcJar = new JarFile(srcFile);
	      for (Enumeration<JarEntry> enumeration = srcJar.entries(); 
	    		  enumeration.hasMoreElements();) {
	          JarEntry entry = enumeration.nextElement();
	          InputStream is = srcJar.getInputStream(entry);
	          int len;
	          while ((len = is.read(buf, 0, buf.length)) != -1) {
	              baos.write(buf, 0, len);
	          }
	          byte[] bytes = baos.toByteArray();
	          String name = entry.getName();
	          if(name.endsWith(".class")){
	              try {
	                  bytes = ByteCodeEncryptor.encrypt(bytes);
	              } catch (Exception e) {
	                  e.printStackTrace();
	              }
	          }
	          JarEntry ne = new JarEntry(name);
	          dstJar.putNextEntry(ne);
	          dstJar.write(bytes);
	          baos.reset();
	      }
	      srcJar.close();
	      dstJar.close();
	      dstFos.close();
	    } catch (Exception e) {
	      e.printStackTrace();
	    }
	  }
}
