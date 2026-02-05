package srcs.interpretor;

import java.io.File;
import java.io.PrintStream;
import java.lang.reflect.InvocationTargetException;
import java.net.URL;
import java.net.URLClassLoader;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.StringTokenizer;

public class CommandInterpretor {
	private Map<String,Class<? extends Command>> map;
	
	public CommandInterpretor() {
		map = new HashMap<>();
		map.put("cat", Cat.class);
		map.put("echo", Echo.class);
		map.put("deploy", Deploy.class);
		map.put("undeploy", Undeploy.class);
	}
	
	public Class<? extends Command> getClassOf(String cmd) {
		return map.get(cmd);
	}
	
	public void perform(String cmd, PrintStream out) throws Exception {
		if (cmd.length() == 0) return;
		
		StringTokenizer command = new StringTokenizer(cmd);
		List<String> tokens = new ArrayList<>();

		while(command.hasMoreTokens()) {
			tokens.add(command.nextToken());
		}
		
		String commandToLaunch = tokens.get(0);
		
		if (!map.containsKey(commandToLaunch)) {
			throw new CommandNotFoundException();
		}

		Class<? extends Command> cl = map.get(commandToLaunch);
		
		try {	
			if (cl.isMemberClass()) {
				// On fait tokens.subList car le premier argument (qu'on supprime) est le nom de la commande interne			
				Command cls = cl.getConstructor(CommandInterpretor.class, List.class).newInstance(this, tokens.subList(1, tokens.size()));
				cls.execute(out);
			} else {
				Command cls = cl.getConstructor(List.class).newInstance(tokens);
				cls.execute(out);
			}
		} catch (InvocationTargetException e) {
			Throwable cause = e.getCause();
	        if (cause instanceof Exception ex) {
	            throw ex; // rethrow the original exception
	        } else {
	            // unlikely, but if cause is an Error, wrap it
	            throw new RuntimeException(cause);
	        }

		}
	}
	
	// NB: Il est impossible d'utiliser classForName() avec le 3ème argument ici car on n'est pas sûr que la classe soit déjà dans le classpath.
	private class Deploy implements Command {
		private String cmd;
		private Class<? extends Command> cls;
		
		public Deploy(List<String> strings) throws IllegalArgumentException {
			if (strings.size() < 3) {
				throw new IllegalArgumentException();
			}
			
			this.cmd = strings.get(0);
			String path = strings.get(1);
			String absoluteName = strings.get(2);
			
			if (map.containsKey(cmd)) {
				throw new IllegalArgumentException();
			}
			
			try {				
				URL url = new File(path).toURI().toURL();
				URLClassLoader loader = new URLClassLoader(new URL[]{url});
				Class<?> cl = loader.loadClass(absoluteName);
				this.cls = cl.asSubclass(Command.class);
				loader.close();
			} catch (Exception e) {
				throw new IllegalArgumentException();
			}
		}
		
		public void execute (PrintStream out) {
			map.put(cmd, cls);
		}
	}
	
	private class Undeploy implements Command {
		private String cmd;
		
		public Undeploy(List<String> strings) {
			if (strings.size() < 1) {
				throw new IllegalArgumentException();
			}
			
			this.cmd = strings.get(0);
		}
		
		public void execute (PrintStream out) {
			map.remove(cmd);
		}
	}
}
















