package srcs.service;

import java.net.Socket;

public interface Service {
	void execute(Socket connexion);
}
