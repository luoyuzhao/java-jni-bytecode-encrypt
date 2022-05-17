package helloworld;

import java.lang.instrument.ClassFileTransformer;
import java.lang.instrument.IllegalClassFormatException;
import java.lang.instrument.Instrumentation;
import java.security.ProtectionDomain;

public class VmAgent {
	//-agentlib:libname[=options] 
	//-agentpath:pathname[=options]
	//-javaagent:jarpath[=options]
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
