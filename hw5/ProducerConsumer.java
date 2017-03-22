package hw5;

import java.util.Random;

public class ProducerConsumer {

	public BoundedBuffer buf;
	public Double totalConsumed = 0.0;
	public Double totalProduced = 0.0;
	
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
	
	public void produce(Double item) throws InterruptedException{
		synchronized (buf) {
			while(buf.isFull()){
				buf.wait();
			}
			buf.push(item);
			buf.notify();
		}
	}
	
	public static void main(String[] args) {
		ProducerConsumer pc = new ProducerConsumer();
		
		Runnable Producer = new Runnable(){
			public void run(){
				for (int i = 0 ; i < 1000000 ; i++){
					Random random = new Random();
					double bufferElement = random.nextDouble() * 100.0;
					pc.totalProduced += bufferElement;
					try {
						pc.produce(bufferElement);
						if (pc.buf.size() > 1000){System.err.println("Buffer Overflow!");}
					}catch (Exception e){
						System.err.println("Exception thrown in Producer... " + e.toString());
					}
					if ((i+1) % 100000 == 0 && i > 0){
						System.out.printf("Producer: Produced %d items, Cumulative value of produced items = %.3f\n", (i+1), pc.totalConsumed);
					}
					if ((i+1) == 1000000){
						System.out.println("Producer: Finished producing 1,000,000 items");
					}
				}
			}
		};
		
		Runnable Consumer = new Runnable(){
			public void run(){
				for (int i = 0 ; i < 1000000 ; i++){
					try{
						pc.totalConsumed += pc.consume();
						if (pc.buf.size() < 0){System.err.println("Buffer Overdrawn!");}
					}catch (Exception e){
						System.err.println("Exception thrown in Consumer... " + e.toString());
					}
					if ((i+1) % 100000 == 0 && i > 0){
						System.out.printf("Consumer: Consumed %d items, Cumulative value of consumed items = %.3f\n", (i+1), pc.totalConsumed);
					}
					if ((i+1) == 1000000){
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
			
			t_one.join();
			t_two.join();
		}catch (Exception e){
			System.err.println("Exception thrown while creating threads... " + e.toString());
		}
		
		System.out.println("Exiting!");
	}
}
