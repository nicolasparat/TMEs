package srcs.persistance;

import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.lang.reflect.InvocationTargetException;

public class PersistanceSauvegardable {
	public static void save(String fichier, Sauvegardable s) throws IOException {
		try (DataOutputStream dos = new DataOutputStream(new FileOutputStream(fichier))) {
			dos.writeUTF(s.getClass().getCanonicalName());
			s.save(dos);
		}
	}
	
	public static Sauvegardable load(String fichier) throws IOException, ClassNotFoundException, InstantiationException, IllegalAccessException, IllegalArgumentException, InvocationTargetException, NoSuchMethodException, SecurityException {
		try (DataInputStream dis = new DataInputStream(new FileInputStream(fichier))) {
			String nomClasse = dis.readUTF();
			Class<?> cl = Class.forName(nomClasse);
			Class <? extends Sauvegardable> cls = cl.asSubclass(Sauvegardable.class);
			// Je passe dis ici, le prof passait fis. Ce serait un peu plus optimisé (évite de re-wrapper le dis dans un dis, dans le constructeur de l'objet)
			// mais ça ne change pas grand chose et j'ai la flemme de refactorer le code.
			Sauvegardable s = cls.getConstructor(InputStream.class).newInstance(dis);
			return s;
		}
	}
}
