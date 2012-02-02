package com.gclue.gl;

public class HttpResponse {
//	private int id;
	private int status;
	private byte[] data;
//	private String[] header;
	
	public void setStatusCode(int code) {
		status = code;
	}
	
	public int getStatusCode() {
		return status;
	}
	
	public void setBody(byte[] data) {
		this.data = data;
	}
	
	public int getLength() {
		if (data == null) {
			return 0;
		}
		return data.length;
	}
	
	public byte[] getBody() {
		return data;
	}
}
