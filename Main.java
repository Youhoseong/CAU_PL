package dadadada;

import java.util.*;

class A {
	String message;
	
	A() {
		message ="A";
	}
	
	void methodExample() {
		System.out.println("Programming Language class printed by " + message );
	}
				
}


class B extends A {
	static void methodExample(char B) {
		System.out.println("Programming Language class printed by "+ B);
	}
	
}

class C extends B {
	static void methodExample(String C) {
		System.out.println("Programming Language class printed by " + C );
	}
	
	
}


class Main {

	public static void main(String args[]) {
		A classA =  new C();
		B classB =  new B();
		C classC =  new C();
		
		
		classA.methodExample();
		((B)classA).methodExample('B');
		((C)classA).methodExample("C");
	}
	
	
}
