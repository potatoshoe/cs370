// Josh Mau
// Mar. 22, 2017
// BoundedBuffer -- Used as buffer of max 1,000
// 				items for ProducerConsumer Class.
// 				retains current size and circular
// 				buffer using in and out index with %.

package hw5;

public class BoundedBuffer {

	// BUFFER_SIZE initialized to 1,000: final value, in and out
	// indexes to implement FIFO queue. Size to be sure 1,000 is 
	// not exceeded and the size never goes below 0. Array of Doubles
	// is the actual buffer.
	private final int BUFFER_SIZE = 1000;
	private int size = 0;
	private int in = 0;
	private int out = 0;
	private double[] buffer = new double[BUFFER_SIZE];
	
	// Incremement the size because an item was added.
	// set the buffer[in] index to the item generated and 
	// advance the index for next use. return item for running total
	public double push(double item){
		size++;
		buffer[in] = item;
		in = (in + 1) % BUFFER_SIZE;
		return item;
	}
	
	// Decrement the size because item was removed.
	// remove item from buffer[out] to implement FIFO queue.
	// Previous value of item is returned for running total.
	public double pop(){
		size--;
		int temp = out;
		out = (out + 1) % BUFFER_SIZE;
		return buffer[temp];
	}

	// To check for Buffer Overflow or Overdraw
	public int size(){return this.size;}
	
	// Consumer should wait when the buffer is empty
	// (nothing to consume)
	public boolean isEmpty(){
		if (this.size == 0)
			return true;
		return false;
	}
	
	// Producer should wait when the buffer is full
	// (no room to produce)
	public boolean isFull(){
		if (this.size == BUFFER_SIZE)
			return true;
		return false;
	}
}
