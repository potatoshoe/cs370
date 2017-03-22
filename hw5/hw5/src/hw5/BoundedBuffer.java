package hw5;

public class BoundedBuffer {

	private final int BUFFER_SIZE = 1000;
	private int size = 0;
	private int in = 0;
	private int out = 0;
	private double totalConsumed = 0.0;
	private double totalProduced = 0.0;
	private double[] buffer = new double[BUFFER_SIZE];
	
	public void push(double item){
		size++;
		buffer[in] = item;
		in = (in + 1) % BUFFER_SIZE;
		this.totalProduced += item;
		System.out.println("Producer total: " + totalProduced);
	}
	
	public double pop(){
		size--;
		int temp = out;
		out = (out + 1) % BUFFER_SIZE;
		this.totalConsumed += buffer[temp];
		System.out.println("Consumer total: " + totalConsumed);
		return buffer[temp];
	}

	public int size(){return this.size;}
	public boolean isEmpty(){
		if (this.size == 0)
			return true;
		return false;
	}
	public boolean isFull(){
		if (this.size == BUFFER_SIZE)
			return true;
		return false;
	}
	public double totalConsumed(){
		return this.totalConsumed;
	}
	public double totalProduced(){
		return this.totalProduced;
	}
}
