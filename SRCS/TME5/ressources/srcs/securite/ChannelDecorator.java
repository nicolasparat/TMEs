package srcs.securite;

import java.io.IOException;
import java.net.InetAddress;

public class ChannelDecorator implements Channel {
	private Channel decorated;
	
	public ChannelDecorator(Channel decorated) {
		this.decorated = decorated;
	}

	public void send(byte[] bytesArray) throws IOException {
		decorated.send(bytesArray);
	}

	public byte[] recv() throws IOException {
		return decorated.recv();
	}

	public InetAddress getRemoteHost() {
		return decorated.getRemoteHost();
	}

	public int getRemotePort() {
		return decorated.getRemotePort();
	}

	public InetAddress getLocalHost() {
		return decorated.getLocalHost();
	}

	public int getLocalPort() {
		return decorated.getLocalPort();
	}
}
