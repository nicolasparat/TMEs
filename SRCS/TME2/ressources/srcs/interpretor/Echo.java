package srcs.interpretor;

import java.io.PrintStream;
import java.util.List;

public class Echo implements Command {
	private List<String> strings;
	
	public Echo(List<String> strings) {
		this.strings = strings;
	}
	
	public void execute (PrintStream out) {
		out.print(strings.get(1));

		for (int i = 2; i < strings.size(); i++) {
			out.print(" " + strings.get(i));
		}
	}
}
