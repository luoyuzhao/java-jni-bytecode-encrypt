package helloworld;

public class ByteCodeEncryptor {
	static {
		System.loadLibrary("your C++ dll name");
	}
	public native static byte[] encrypt(byte[] text);
	public native static byte[] decrypt(byte[] text);
}
