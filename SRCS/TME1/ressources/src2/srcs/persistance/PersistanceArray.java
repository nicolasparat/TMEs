package srcs.persistance;

import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;

public class PersistanceArray {
	public static void saveArrayInt(String f, int[] tab) {
		try (DataOutputStream fichier = new DataOutputStream(new FileOutputStream(f))) {
			fichier.writeInt(tab.length);
			for (int item : tab ) {
				fichier.writeInt(item);
			}
		} catch (IOException e) {}	
	}
	
	public static int[] loadArrayInt(String fichier) throws IOException {
		try (DataInputStream f = new DataInputStream(new FileInputStream(fichier))) {
				int taille = f.readInt();
				int[] result = new int[taille];
				for (int i = 0; i < taille; i++) {
					int a = f.readInt();
					result[i] = a;
				}
				return result;
		}
	}
}
