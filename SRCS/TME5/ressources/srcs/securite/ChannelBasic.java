package srcs.securite;

import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.net.InetAddress;
import java.net.Socket;

public class ChannelBasic implements  Channel {
	private Socket sock;
	private ObjectOutputStream os;
	private ObjectInputStream is;
	
	public ChannelBasic(Socket sock) throws IOException {
		this.sock = sock;
		this.os = new ObjectOutputStream(this.sock.getOutputStream());
		this.is = new ObjectInputStream(this.sock.getInputStream());
	}
	
	public void send(byte[] bytesArray) throws IOException {
		this.os.writeObject(bytesArray);
		this.os.flush();
	}
	
	public byte[] recv() throws IOException {
		try {			
			return (byte[]) this.is.readObject();
		} catch (ClassNotFoundException e) {
			throw new IllegalStateException(e);
		}
	}
	
	public InetAddress getRemoteHost() {
		return this.sock.getInetAddress();
	}
	
	public int getRemotePort() {
		return this.sock.getPort();
	}
	
	public InetAddress getLocalHost() {
		return this.sock.getLocalAddress();
	}
	
	public int getLocalPort() {
		return this.sock.getLocalPort();
	}
}
