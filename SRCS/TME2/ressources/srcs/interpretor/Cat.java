package srcs.interpretor;

import java.io.File;
import java.io.PrintStream;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.List;

public class Cat implements Command {
	private String path;
	
	// NB : Toutes les exceptions sont emballées dans une InvocationTargetException si on utilise la réflexivité pour appeler le constructeur.	
	public Cat(List<String> args) throws IllegalArgumentException {
		
		if (args.size() < 2) {
			throw new IllegalArgumentException();
		}
		
		String path = args.get(1);
		File file = new File(path);
		
		if(!file.isFile()) {
			throw new IllegalArgumentException();
		}
		
		this.path = path;
	}
	
	public void execute (PrintStream out) throws Exception {
		try (var stream = Files.lines(Path.of(path))) {
			stream.forEach(out::println);
		}
	}
}
