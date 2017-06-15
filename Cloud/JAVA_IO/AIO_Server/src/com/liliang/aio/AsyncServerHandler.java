package com.liliang.aio;

import java.io.IOException;
import java.net.InetSocketAddress;
import java.nio.channels.AsynchronousServerSocketChannel;
import java.util.concurrent.CountDownLatch;

public class AsyncServerHandler implements Runnable {
	
	private int port;
	public CountDownLatch latch;
	public AsynchronousServerSocketChannel asynchronousServerSocketChannel;
	
	
	
	public AsyncServerHandler(int port){
		try{
			asynchronousServerSocketChannel = AsynchronousServerSocketChannel.open();
			asynchronousServerSocketChannel.bind(new InetSocketAddress(port));			
		}catch(IOException e){
			e.printStackTrace();
		}
	}
	
	@Override
	public void run() {
		// TODO Auto-generated method stub
		latch = new CountDownLatch(1);
		asynchronousServerSocketChannel.accept(this, new AcceptCompletionHandler());
		try{
			latch.await();
		}catch(InterruptedException e){
			e.printStackTrace();
		}
	}

}
