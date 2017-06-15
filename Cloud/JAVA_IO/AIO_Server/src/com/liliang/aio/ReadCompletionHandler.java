package com.liliang.aio;

import java.io.IOException;
import java.io.UnsupportedEncodingException;
import java.nio.ByteBuffer;
import java.nio.channels.AsynchronousSocketChannel;
import java.nio.channels.CompletionHandler;

public class ReadCompletionHandler implements CompletionHandler<Integer, ByteBuffer> {

	private AsynchronousSocketChannel channel;
	
	public ReadCompletionHandler(AsynchronousSocketChannel channel){
		if(this.channel == null) this.channel = channel;
	}
	
	@Override
	public void completed(Integer result, ByteBuffer attachment) {
		// TODO Auto-generated method stub
		attachment.flip();
		byte []body = new byte[attachment.remaining()];
		attachment.get(body);
		try{
			String req = new String(body, "UTF-8");
			System.out.println("Server received:"+req);
			sendClientContent("How are you!");
		}catch(UnsupportedEncodingException e){
			e.printStackTrace();
		}
	}

	@Override
	public void failed(Throwable exc, ByteBuffer attachment) {
		// TODO Auto-generated method stub
		try {
			this.channel.close();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
	
	private void sendClientContent(String content){
		
		byte[] bytes = content.getBytes();
		ByteBuffer writeBuffer = ByteBuffer.allocate(bytes.length);
		writeBuffer.put(bytes);
		writeBuffer.flip();
		channel.write(writeBuffer, writeBuffer, new CompletionHandler<Integer, ByteBuffer>(){
			@Override
			public void completed(Integer result, ByteBuffer attachment) {
				// TODO Auto-generated method stub
				if(attachment.hasRemaining())
					channel.write(attachment, attachment, this);
			}

			@Override
			public void failed(Throwable exc, ByteBuffer attachment) {
				// TODO Auto-generated method stub
				try{
					channel.close();
				}catch(IOException e){
					e.printStackTrace();
				}
			}
			
		});
	}
}
