import java.util.ArrayList;

/**********************************************
 * Highest Priority First (non-preemptive)
 * 
 * The highest priority process runs
 * until it is complete
 *
 * @author Team: Inception
 * CS149
 * 
 **********************************************/
public class HPF {
	private ArrayList<Process> processData;
    private ArrayList<Process> runnableData;
    private ArrayList<Character> hpf;
    private float cnt;
    
    public HPF(ArrayList<Process> p) {
    	this.processData = (ArrayList<Process>) p.clone();
    	this.runnableData = new ArrayList<Process>();
    	this.hpf = new ArrayList<Character>();
    	this.cnt = 0;

    	sort();
    	createList();
    }
    
    /**
     * Gets the Array of processes
     * in their quantum
     */
    public ArrayList<Character> gethpf() {
    	return hpf;
    }
    
    /**
     * Sorts the process data
     * by arrival time
     */
    public void sort() {
    	for (int i = 0; i < processData.size()-1; i++)
    	{
    		for (int j = 1; j < processData.size(); j++ )
    		{
    			if (processData.get(j).getArrivalTime() < processData.get(i).getArrivalTime())
    			{
    				Process k = processData.get(i);
    				processData.set(i, processData.get(j));
    				processData.set(j, k);
    			}
    		}
    	}
    }
    
    
    /**
     * Creates the list for the processes
     * puts them in their quantum 
     */
    public void createList() { 	    	
    	while( (processData != null && runnableData != null) || cnt < 100 )
    	{	   		
    		runtimeProcesses();
        	
        	Process run = highPriority();
        	while ( run.getRunTime() != 0 )
        	{
        		hpf.add( run.getName() );
        		cnt++;
        		run.setRunTime( run.getRunTime() - 1 );
        	}
    		
    		runnableData.remove(run);
    	}
    	
    }
    
    /**
     * Adds the Processes
     * at their interval time
     * 
     */
    public void runtimeProcesses() {
    	ArrayList<Integer> remove = new ArrayList<Integer>();
    	for(int i = 0; i < processData.size(); i++)
    	{
    		if( processData.get(i).getArrivalTime() < cnt ) 
    		{
    			runnableData.add(processData.get(i));
    			remove.add(i);
    		}
    	}
    	
    	for(int j = remove.size()-1; j >= 0; j--)
    	{	processData.remove(remove.get(j));	}
    }
    
    /**
     * Finds the highest priority
     * process in the runnable processes
     * @return A process with the highest
     * priority
     */
    public Process highPriority() {
    	Process highP = new Process();
    	
    	for(Process P: runnableData)
    	{
    		if(P.getPriority() > highP.getPriority())
    		{	highP = P;	}
    	}
    	
    	return highP;
    }
}
