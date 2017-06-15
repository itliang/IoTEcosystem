package com.liliang.aio;

import java.io.IOException;

public class AioClient {

	public static void main(String[] args) throws IOException {
		// TODO Auto-generated method stub
		AsyncClientHandler client = new AsyncClientHandler("127.0.0.1", 8080);
		new Thread(client, "AIO-AyncClientHandler-1").start();
	}
}
