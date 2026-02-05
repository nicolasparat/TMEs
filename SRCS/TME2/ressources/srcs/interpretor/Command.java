package srcs.interpretor;

import java.io.PrintStream;

public interface Command {
	public void execute (PrintStream out) throws Exception;
}
