import java.util.Random;

public class PrintTask implements Runnable {

    private final int sleepTime;
    private final String taskName;
    private final static Random generator = new Random();

    public PrintTask(String name) {
        taskName = name;
        sleepTime = generator.nextInt(5000);
    }

    @Override
    // method run contains the code that a thread will execute
    public void run() {
        try // put thread to sleep for sleepTime amount of time
        {
            System.out.printf("%s going to sleep for %d milliseconds.\n", taskName, sleepTime);
            Thread.sleep(sleepTime); // put thread to sleep
        } // end try
        catch (InterruptedException exception) {
            System.out.printf("%s %s\n", taskName, "terminated prematurely due to interruption");
        } // end catch
          // print task name
        System.out.printf("%s done sleeping\n", taskName);
    } // end method run
} // end class PrintTask
