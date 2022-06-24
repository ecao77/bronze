import java.io.*;
import java.util.*;
public class rut {

	public static void main(String[] args) throws IOException {
		/*
		 * 6
		E 3 5
		N 5 3
		E 4 6
		E 10 4
		N 11 2
		N 8 1
		 */
		BufferedReader br = new BufferedReader(new InputStreamReader(System.in));
		StringTokenizer tokens = new StringTokenizer(br.readLine());
		int numCows = Integer.parseInt(tokens.nextToken());
		boolean[][] field = new boolean[20000][20000];
		for(int i = 0; i < 20000; i++) {
			for(int j = 0; j < 20000; j++) {
				field[i][j] = true;
			}
		}
		
		String[] direction = new String[numCows];
		int[] x = new int[numCows];
		int[] y = new int[numCows];
		int[] xOriginal = new int[numCows];
		int[] yOriginal = new int[numCows];
		
		boolean[] moving = new boolean[numCows];
		Arrays.fill(moving, true);
		
		int[] steps = new int[numCows];
		Arrays.fill(steps, 1);
		//steps[1] = 2;
		for(int i = 0; i < numCows; i++) {
			field[x[i]][y[i]] = false;
		}
		
		for(int i = 0; i < numCows; i++) {
			tokens = new StringTokenizer(br.readLine());
			direction[i] = tokens.nextToken();
			x[i] = Integer.parseInt(tokens.nextToken());
			y[i] = Integer.parseInt(tokens.nextToken());
			xOriginal[i] = x[i];
			yOriginal[i] = y[i];
		}
		
		for(int time = 0; time < 10000; time++) {
			for(int i = 0; i < numCows; i++) {
				
				if(direction[i].equals("N") && moving[i]) {
					//y + 1
					if(field[x[i]][y[i]+1]) {
						field[x[i]][y[i]+1] = false;
						y[i]++;
						steps[i]++;
					}
					else {
						if (intersectsAtSameTime(time,i,direction,xOriginal,yOriginal)) {
							//System.out.println("the system returned true for time " + i);
							field[x[i]][y[i]+1] = false;
								moving[i] = true;
								steps[i]++;
								y[i]++;
							}
						else moving[i] = false;
					}
				}
				if(direction[i].equals("E") && moving[i]) {
					//x+1
					if(field[x[i]+1][y[i]]) {
						field[x[i]+1][y[i]] = false;
						x[i]++;
						steps[i]++;
					}
					else {
						if (intersectsAtSameTime(time,i,direction,xOriginal,yOriginal)) {
						//System.out.println("the system returned true for time " + i);
							field[x[i]+1][y[i]] = false;
							moving[i] = true;
							steps[i]++;
							x[i]++;
						}
						else moving[i] = false;
					}
				}
			}
		}
		
		if(allStopped(moving)) {
			for(int i = 0; i < numCows; i++) {
				System.out.println(steps[i]);
			}
		}
		else {
			for(int i = 0; i < numCows; i++) {
				if(moving[i]) {
					System.out.println("Infinity");
				}
				else {
					
					System.out.println(steps[i]);
				}
			}
		}
		
		//a cow can move if
		//not outside field
		//moving spot has grass
		
	}

private static boolean intersectsAtSameTime(int time, int m, String[] direction, int[] x, int[] y) {
		
			for(int j = 0; j < x.length; j++) {
				int mX;
				int mY;
				int jX;
				int jY;
				if(!direction[m].equals(direction[j])) {
					if(direction[m].equals("N")) {
						mX = x[m];
						mY = y[m] + time + 1;
						jX = x[j] + time + 1;
						jY = y[j];
					}
					else {
						mX = x[m] + time + 1;
						mY = y[m];
						jX = x[j];
						jY = y[j] + time + 1;
					}
					
					if(mX == jX && mY == jY) {
						return true;
					}
					
				}
			}
		
		return false;
	}
	

	private static boolean allStopped(boolean[] moving) {
		boolean ret = false;
		for(int i = 0; i < moving.length; i++) {
			if(moving[i]) {
				ret = true;
			}
		}
		return !ret;
	}
}

