// Josh Mau
// Mar. 22, 2017
// Homework 5 -- Multithreading Producer/Consumer
// 			problem. Producing 1,000,000 items and
// 			consuming 1,000,000 items using a buffer
//			of 1,000 items. Using wait() and notify()

package hw5;

import java.util.Random;

public class ProducerConsumer {

	public BoundedBuffer buf;
	public Double totalConsumed = 0.0;
	public Double totalProduced = 0.0;
	public final int MIL = 1000000;
	
	public ProducerConsumer(){
		buf = new BoundedBuffer();
	}
	
	public Double consume() throws InterruptedException {
		synchronized (buf) {
			while(buf.isEmpty()){
				buf.wait();
			}
			Double consumedItem = buf.pop();
			buf.notify();
			return consumedItem;
		}
	}
	
	public Double produce(Double item) throws InterruptedException{
		synchronized (buf) {
			while(buf.isFull()){
				buf.wait();
			}
			Double producedItem = buf.push(item);
			buf.notify();
			return producedItem;
		}
	}
	
	public static void main(String[] args) {
		ProducerConsumer pc = new ProducerConsumer();
		
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
		
		System.out.println("Exiting!");
	}
}
