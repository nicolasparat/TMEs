package srcs.persistance;

import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;

import srcs.banque.Compte;

public class PersistanceCompte {
	public static void saveCompte(String f, Compte e) throws IOException {
		e.save(new FileOutputStream(f));
	}
	
	public static Compte loadCompte(String f) throws IOException {
		return new Compte(new FileInputStream(f));
	}
}
