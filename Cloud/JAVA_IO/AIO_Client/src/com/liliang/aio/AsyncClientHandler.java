package com.liliang.aio;

import java.io.IOException;
import java.io.UnsupportedEncodingException;
import java.net.InetSocketAddress;
import java.nio.ByteBuffer;
import java.nio.channels.AsynchronousSocketChannel;
import java.nio.channels.CompletionHandler;
import java.util.concurrent.CountDownLatch;

public class AsyncClientHandler implements CompletionHandler<Void, AsyncClientHandler>, Runnable {

	private AsynchronousSocketChannel client;
	private CountDownLatch latch;
	
	private int port;
	private String host;
	
	public AsyncClientHandler(String host, int port) throws IOException{
		this.port = port;
		this.host = host;
		client = AsynchronousSocketChannel.open();
	}
	
	@Override
	public void run() {
		// TODO Auto-generated method stub
		latch = new CountDownLatch(1);
		client.connect(new InetSocketAddress(host, port), this, this);
		try{
			latch.await();
		}catch(InterruptedException e){
			e.printStackTrace();
		}
	}

	@Override
	public void completed(Void result, AsyncClientHandler attachment) {
		// TODO Auto-generated method stub
		byte []req = "Hello, Server".getBytes();
		ByteBuffer writeBuffer = ByteBuffer.allocate(req.length);
		writeBuffer.put(req);
		writeBuffer.flip();
		client.write(writeBuffer, writeBuffer, new CompletionHandler<Integer, ByteBuffer>(){

			@Override
			public void completed(Integer result, ByteBuffer attachment) {
				// TODO Auto-generated method stub
				if(attachment.hasRemaining()){
					client.write(attachment, attachment, this);
				} else {
					ByteBuffer readBuffer = ByteBuffer.allocate(1024);
					client.read(readBuffer, readBuffer, new CompletionHandler<Integer, ByteBuffer>(){
						@Override
						public void completed(Integer result, ByteBuffer attachment) {
							// TODO Auto-generated method stub
							attachment.flip();
							byte[] bytes = new byte[attachment.remaining()];
							attachment.get(bytes);
							String body;
							try {
								body = new String(bytes, "UTF-8");
								System.out.println("Receive from server:"+body);
								latch.countDown();
							} catch (UnsupportedEncodingException e) {
								// TODO Auto-generated catch block
								e.printStackTrace();
							}							
						}

						@Override
						public void failed(Throwable exc, ByteBuffer attachment) {
							// TODO Auto-generated method stub
							try {
								client.close();
								latch.countDown();
							} catch (IOException e) {
								// TODO Auto-generated catch block
								e.printStackTrace();
							}							
						}						
					});
				}
			}

			@Override
			public void failed(Throwable exc, ByteBuffer attachment) {
				// TODO Auto-generated method stub
				try {
					client.close();
					latch.countDown();
				} catch (IOException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}	
			}
			
		});
	}

	@Override
	public void failed(Throwable exc, AsyncClientHandler attachment) {
		// TODO Auto-generated method stub
		exc.printStackTrace();
		try {
			client.close();
			latch.countDown();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}		
	}

}
