package com.liliang.nio;

import java.io.IOException;
import java.net.InetSocketAddress;
import java.nio.ByteBuffer;
import java.nio.channels.SelectionKey;
import java.nio.channels.Selector;
import java.nio.channels.SocketChannel;
import java.util.Iterator;
import java.util.Set;

public class NioClientHandler implements Runnable {

	private Selector selector;
	private SocketChannel socketChannel;
	
	private String host;
	private int port;
	
	private boolean stop;
	
	public NioClientHandler(String host, int port){
		this.host = host;
		this.port = port;
		this.stop = false;
		try{
			selector = Selector.open();
			socketChannel = SocketChannel.open();
			socketChannel.configureBlocking(false);
		}catch (IOException e){
			e.printStackTrace();
			System.exit(1);
		}
	}
	
	
	@Override
	public void run() {
		// TODO Auto-generated method stub
		try{
			boolean result = socketChannel.connect(new InetSocketAddress(host, port));
			if(result){
				System.out.println("Connection Synchronous");
				socketChannel.register(selector, SelectionKey.OP_READ);
				sendToServer(socketChannel, "Hello Server!");
			}else{
				System.out.println("Connection Asynchronous");
				socketChannel.register(selector, SelectionKey.OP_CONNECT);
			}
		}catch(IOException e){
			e.printStackTrace();
		}
		while(!stop){
			try{
				//System.out.println("Client");
				selector.select(1000);
				Set<SelectionKey> selectedKeys = selector.selectedKeys();
				Iterator<SelectionKey> it = selectedKeys.iterator();
				SelectionKey key = null;
				while(it.hasNext()){
					key = it.next();
					it.remove();
					try{
						receiveFromServer(key);						
					}catch(IOException e){
						e.printStackTrace();
						if(key != null){
							key.cancel();
							if(key.channel()!=null) key.channel().close();
						}
					}
				}
				
			}catch(Exception e){
				e.printStackTrace();
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
	
	private void sendToServer(SocketChannel sc, String content) throws IOException{
		byte []Bytes = content.getBytes();
		ByteBuffer writeBuffer = ByteBuffer.allocate(Bytes.length);
		writeBuffer.put(Bytes);
		writeBuffer.flip();
		sc.write(writeBuffer);
		if(!writeBuffer.hasRemaining()){
			System.out.println("Send to server again");
		}
	}

	private void receiveFromServer(SelectionKey key) throws IOException{
		SocketChannel sc = (SocketChannel)key.channel();
		if(key.isConnectable()){
			if(sc.finishConnect()){
				sc.register(selector, SelectionKey.OP_READ);
				sendToServer(sc, "What will server send?");
			}
		}else if(key.isReadable()){
			ByteBuffer readBuffer = ByteBuffer.allocate(1024);
			int readBytes = sc.read(readBuffer);
			if(readBytes > 0){
				readBuffer.flip();
				byte[] bytes = new byte[readBuffer.remaining()];
				readBuffer.get(bytes);
				String body = new String(bytes, "UTF-8");
				System.out.println("Client received :"+body);
				this.stop = true;
			}else if(readBytes < 0){
				System.out.println("Connection lost");
				key.cancel();
				sc.close();
			}
			
			
		}
	}
}
