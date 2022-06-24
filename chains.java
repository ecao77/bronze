import java.io.*;
import java.util.*;
public class chains {

	public static void main(String[] args) throws IOException {
		BufferedReader br = new BufferedReader(new InputStreamReader(System.in));
		StringTokenizer tokens = new StringTokenizer(br.readLine());
		int numDaisies = Integer.parseInt(tokens.nextToken());
		int total = 0;
		
		int[] daisies = new int[numDaisies];
		tokens = new StringTokenizer(br.readLine());
		for(int i = 0; i < numDaisies; i++) {
			daisies[i] = Integer.parseInt(tokens.nextToken());
		}
		
		for(int i = 0; i < numDaisies; i++) {
			for(int j = i; j < numDaisies; j++) {
				if(averageExists(i, j, daisies)) {
					total++;
				}
			}
		}
		
		System.out.println(total);

	}

	private static boolean averageExists(int x, int y, int[] daisies) {
		int sum = 0;
		int numIterated = 0;
		for(int i = x; i <= y; i++) {
			numIterated++;
			sum = sum + daisies[i];
		}
		if(sum % numIterated == 0) {
			int dividend = sum / numIterated;
			for(int i = x; i <= y; i++) {
				if(daisies[i] == dividend) {
					return true;
				}
			}
		}
		else {
			return false;
		}
		
		return false;
	}
	

}
