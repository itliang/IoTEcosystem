package com.liliang.nio;

import java.io.IOException;
import java.net.InetSocketAddress;
import java.nio.ByteBuffer;
import java.nio.channels.SelectionKey;
import java.nio.channels.Selector;
import java.nio.channels.ServerSocketChannel;
import java.nio.channels.SocketChannel;
import java.util.Iterator;
import java.util.Set;

public class MultiplexerServer implements Runnable {
	private ServerSocketChannel serverChannel;
	private Selector selector;
	
	private volatile boolean stop;
	
	public MultiplexerServer(int port){
		try{
			selector = Selector.open();
			serverChannel = ServerSocketChannel.open();
			serverChannel.configureBlocking(false);
			serverChannel.bind(new InetSocketAddress(port), 1024);
			serverChannel.register(selector, SelectionKey.OP_ACCEPT);
		}catch(IOException e){
			e.printStackTrace();
			System.exit(1);
		}
	}
	
	public void stop(){
		this.stop = true;
	}
	
	@Override
	public void run() {
		// TODO Auto-generated method stub
		while(!stop){
			try{				
				selector.select(1000); //Wake up in a second
				Set<SelectionKey> selecteKeys = selector.selectedKeys();
				Iterator<SelectionKey> it = selecteKeys.iterator();
				SelectionKey key = null;
				while(it.hasNext()){
					key = it.next();
					it.remove();
					try{
						System.out.println("Client Request");
						handleClientRequest(key);						
					}catch(Exception e){
						if(key != null){
							key.cancel();
							if(key.channel()!=null){
								key.channel().close();
							}
						}
					}
				}
			}catch(Throwable t){
				t.printStackTrace();
				System.exit(1);
			}			
		}
		if(selector != null){
			try{
				selector.close();
			}catch(IOException e){
				e.printStackTrace();
			}
		}
	}
	
	private void handleClientRequest(SelectionKey key) throws IOException{
		if(key.isValid()){
			if(key.isAcceptable()){
				//Accept new connection
				ServerSocketChannel ssc = (ServerSocketChannel)key.channel();
				SocketChannel sc = ssc.accept();
				sc.configureBlocking(false);
				sc.register(selector, SelectionKey.OP_READ);
				System.out.println("Accept connection request");
			}else if(key.isReadable()){
				//Read data
				System.out.println("Read data");
				SocketChannel sc = (SocketChannel)key.channel();
				ByteBuffer readBuffer = ByteBuffer.allocate(1024);
				int readBytes = sc.read(readBuffer); //None block, Check with return value
				if(readBytes > 0){
					readBuffer.flip(); //limit == position, position = 0;
					byte []bytes = new byte[readBuffer.remaining()];
					readBuffer.get(bytes);
					String body = new String(bytes, "UTF-8");
					System.out.println("Server received:"+body);
					
					String currentTime = new java.util.Date(System.currentTimeMillis()).toString();
					sendToClient(sc, currentTime);
					sendToClient(sc, "How are you!");					
				}else if (readBytes < 0){
					//Linkage lost, release resource
					key.cancel();
					sc.close();
				}else if (readBytes == 0){
					//Normal case, ignore
				}
			}
		}
	}
	
	private void sendToClient(SocketChannel channel, String content) throws IOException{
		if(content !=null && content.trim().length() > 0){
			byte []bytes = content.getBytes();
			ByteBuffer replyBuffer = ByteBuffer.allocate(bytes.length);
			replyBuffer.put(bytes);
			replyBuffer.flip();
			channel.write(replyBuffer);
		}
	}
}
