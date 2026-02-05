package srcs.persistance;

import java.io.IOException;
import java.io.OutputStream;

public interface Sauvegardable {
	void save(OutputStream os) throws IOException;
}
