// Josh Mau
// Mar. 22, 2017
// ProducerConsumer -- Multithreading Producer/Consumer
// 			problem. Producing 1,000,000 items and
// 			consuming 1,000,000 items using a buffer
//			of 1,000 items. Using wait() and notify()

package hw5;

import java.util.Random;

public class ProducerConsumer {

	// Each ProducerConsumer problem has a BoundedBuffer
	// a Sum totalConsumed and a Sum totalProduced (at 1,000,000 
	// totalConsumed == totalProduced). final int MIL for readability.
	public BoundedBuffer buf;
	public Double totalConsumed = 0.0;
	public Double totalProduced = 0.0;
	public final int MIL = 1000000;
	
	// Constructor -- initialized empty BoundedBuffer
	public ProducerConsumer(){
		buf = new BoundedBuffer();
	}
	
	// Method consume -- uses synchronized statement with buffer
	// For Consumer: Wait while the buffer is empty (nothing to consume)
	// Acquire buffer lock and consume FIFO item, and notify Producer of
	// Buffer availability.
	public Double consume() throws InterruptedException {
		synchronized (buf) {
			
			// Entry Section
			while(buf.isEmpty()){
				buf.wait();
			}
			
			// *** Critical Section ***
			Double consumedItem = buf.pop();
			
			// Exit Section
			buf.notify();
			
			return consumedItem;
		}
	}
	
	// Method produce -- uses synchronized statement with buffer
	// For Producer: Wait while the buffer is full (no room to produce)
	// Acquire buffer lock and produce FIFO item, and notify Consumer of
	// Buffer availability.
	public Double produce(Double item) throws InterruptedException{
		synchronized (buf) {
			
			// Entry Section
			while(buf.isFull()){
				buf.wait();
			}
			
			// *** Critical Section ***
			Double producedItem = buf.push(item);
			
			// Exit Section
			buf.notify();
			
			return producedItem;
		}
	}
	
	public static void main(String[] args) {
		ProducerConsumer pc = new ProducerConsumer();
		
		// Thread t_one is the Producer, This is the .start() method for
		// the Producer Thread. Produces 1,000,000 random doubles and attempts
		// to add them to the buffer when space is available.
		Runnable Producer = new Runnable(){
			public void run(){
				for (int i = 0 ; i < pc.MIL ; i++){
					Random random = new Random();
					double bufferElement = random.nextDouble() * 100.0;
					
					try {
						pc.totalProduced += pc.produce(bufferElement);
						if (pc.buf.size() > 1000)
							System.err.println("Buffer Overflow!");
					}catch (Exception e){
						System.err.println("Exception thrown in Producer... " + e.toString());
					}
					
					if ((i+1) % 100000 == 0 && i > 0 && (i+1) < pc.MIL){
						System.out.printf("Producer: Generated %d00,000 items, Cumulative value of generated items=%.3f\n", (i+1)/100000, pc.totalProduced);
					}
					if ((i+1) == pc.MIL){
						System.out.printf("Producer: Generated 1,000,000 items, Cumulative value of generated items=%.3f\n", pc.totalProduced);
						System.out.println("Producer: Finished generating 1,000,000 items");
					}
				}
			}
		};
		
		// Thread t_two is the Consumer, This is the .start() method for
		// the Consumer Thread. Consumes 1,000,000 random doubles from the buffer
		// when there is something to consume (buffer is not empty).
		Runnable Consumer = new Runnable(){
			public void run(){
				for (int i = 0 ; i < pc.MIL ; i++){
					try{
						pc.totalConsumed += pc.consume();
						if (pc.buf.size() < 0)
							System.err.println("Buffer Overdrawn!");
					}catch (Exception e){
						System.err.println("Exception thrown in Consumer... " + e.toString());
					}
					
					if ((i+1) % 100000 == 0 && i > 0 && (i+1) < pc.MIL){
						System.out.printf("Consumer: Consumed %d00,000 items, Cumulative value of consumed items=%.3f\n", (i+1)/100000, pc.totalConsumed);
					}
					if ((i+1) == pc.MIL){
						System.out.printf("Consumer: Consumed 1,000,000 items, Cumulative value of consumed items=%.3f\n", pc.totalConsumed);
						System.out.println("Consumer: Finished consuming 1,000,000 items");
					}
				}
			}
		};
		
		// Main section -- creates two threads: a Producer and a Consumer
		// calls .start() on both threads which is the Runnable methods above
		// for the corresponding Thread.
		try {
			Thread t_one = new Thread(Producer);
			Thread t_two = new Thread(Consumer);
			t_one.start();
			t_two.start();
			
			// Wait for both threads to finish before exiting
			t_one.join();
			t_two.join();
		}catch (Exception e){
			System.err.println("Exception thrown while creating threads... " + e.toString());
		}
		
		// Finished; Print final statement
		System.out.println("Exiting!");
	}
}
