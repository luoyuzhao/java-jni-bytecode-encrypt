# java-jni-bytecode-encrypt

java字节码加密解密及agent接入

1.create C++ project import nativefiles change the pkgNames and build

2.put the cppDLL file to jvm lib path

3.Java:import dll(JNI-CALL)

```java
public class ByteCodeEncryptor {
	static {
		System.loadLibrary("your C++ dll name");
	}
	public native static byte[] encrypt(byte[] text);
	public native static byte[] decrypt(byte[] text);
}
```

4.Java:Encrypt jar(-加密一个原始jar包-加密后jar包)

```java
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
```

5.
jvm option 

--方式一 jvm -agentlib:yourCppDLLname  (在C中解密直接使用dll)

RUN: java -agentlib:yourCppDLLname  -jar  encryptedFile.jar(加密后的jar)

--方式二 jvm -javaagent:yourAgentJar  (在Java中解密处理premain)
MANIFEST.MF

```
 Manifest-Version: 1.0
 Premain-Class: helloworld.VmAgent
 Sealed: true
```

```java
public class VmAgent {
	public static void premain(String agentArgs,Instrumentation inst) {
		System.out.print("VMAgent");
		inst.addTransformer(new ClassFileTransformer() {
			@Override
			public byte[] transform(ClassLoader loader, String className, Class<?> classBeingRedefined,
					ProtectionDomain protectionDomain, byte[] classfileBuffer) throws IllegalClassFormatException {
				// TODO Auto-generated method stub
				if(className.contains("helloworld")) {
					System.out.print(className);
					  try {
		                  byte[] res = ByteCodeEncryptor.decrypt(classfileBuffer);
		                  return res;
		              } catch (Exception e) {
		                  e.printStackTrace();
		                  return null;
		              }
				}
				return null;
			}
		});
	}

}
```

RUN: java -javaagent:yourAgentJar  -jar encryptedFile.jar



