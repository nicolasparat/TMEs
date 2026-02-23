package srcs.service;

import java.io.IOException;
import java.lang.reflect.InvocationTargetException;
import java.net.ServerSocket;
import java.net.Socket;

public class ServeurMultiThread {
	private final int listeningPort;
	private Service service;
	private final Class<? extends Service> classe;
	
	
	public ServeurMultiThread(int listeningPort, Class<? extends Service> theirClass) {
		this.listeningPort = listeningPort;
		this.classe = theirClass;
	}
	
	public void listen() throws IllegalStateException {
		try (ServerSocket ss = new ServerSocket(this.listeningPort)) {
			while (true) {
				Socket s = ss.accept();
				Service c = this.getService();
				Thread t = new Thread(() -> c.execute(s));
				t.start();
			}
		} catch (IOException | NoSuchMethodException | InstantiationException | IllegalAccessException | IllegalArgumentException | InvocationTargetException | SecurityException e) {
			System.out.println(e);
		}
	}
	
	private Service getService() throws IllegalStateException, InstantiationException, IllegalAccessException, IllegalArgumentException, InvocationTargetException, NoSuchMethodException, SecurityException {
		if (this.classe.isAnnotationPresent(SansEtat.class)) {
			return this.classe.getConstructor().newInstance();
		}
		
		if (this.classe.isAnnotationPresent(EtatGlobal.class)) {
			if(service==null) {
				service=classe.getConstructor().newInstance();
			}
			return service;
		}
 
		throw new IllegalStateException("Pas d'annotation");
	}
}
