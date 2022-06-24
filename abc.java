import java.io.*;
import java.util.*;

public class abc {

	public static void main(String[] args) throws IOException {
		BufferedReader br = new BufferedReader(new InputStreamReader(System.in));
		int[] arr = new int[7];
		
		int sum = 0;
		StringTokenizer tokens = new StringTokenizer(br.readLine());
		for(int i = 0; i < 7; i++) {
			arr[i] = Integer.parseInt(tokens.nextToken());
			sum = sum + arr[i];
		}
		Arrays.sort(arr);
		
		for(int i = 0; i < 7; i++) {
			for(int j = i + 1; j < 7; j++) {
				for(int z = j + 1; z < 7; z++) {
					if(works(arr[i],arr[j],arr[z],arr)) {
						int[] fin = new int[3];
						fin[0] = arr[i];
						fin[1] = arr[j];
						fin[2] = arr[z];
						Arrays.sort(fin);
						System.out.println(fin[0] + " " + fin[1] + " " + fin[2]);
					}
				}
			}
		}
		
		/*
		int sumOfABC = sum / 4;
		System.out.println(sumOfABC);
		
		
		
		
		int[] ABC = new int[3];
		for(int i = 0; i < 3; i++) {
			ABC[i] = sumOfABC - arr[i+3];
		}
		
		Arrays.sort(ABC);
		for(int i = 0; i < 3; i++) {
			System.out.println(ABC[i]);
		}
		*/
		
		

	}

	private static boolean works(int x, int y, int z, int[] arr) {
			boolean sumXY = false;
			boolean sumYZ = false;
			boolean sumXZ = false;
			boolean totalSum = false;
			
			for(int i = 0; i < 7; i++) {
				if(arr[i] == (x+y)){
					sumXY = true;
				}
				if(arr[i] == (y+z)) {
					sumYZ = true;
				}
				if(arr[i] == (x+z)) {
					sumXZ = true;
				}
				if(arr[i] == (x+y+z)) {
					totalSum = true;
				}
			}
			
			
		return (sumXY && sumYZ && sumXZ && totalSum);
	}

}
