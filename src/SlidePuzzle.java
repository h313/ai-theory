/**
 * @author Haoda Wang
 */
import java.io.*;
import java.lang.*;
import java.util.Scanner;
import java.util.PriorityQueue;

public class SlidePuzzle {
    private static int[][] puzzle = new int[4][4];

    /**
     * Main method that makes the thing actually run
     * @param args arguments that you never use 10/10
     * @throws FileNotFoundException
     */
    public static void main(String[] args) throws FileNotFoundException {
        Scanner s = new Scanner(new BufferedReader(new FileReader("puzzle.txt")));
        for(int i = 0; i < 4; i++)
            for(int it = 0; it < 4; it++)
                puzzle[i][it] = (int) s.next().charAt(0) - 65 < 16 ? (int) s.next().charAt(0) - 65 : -1;
        System.out.println("Puzzle read!");
    }

    /**
     * Gets the manhattan distance from an actual position to the target
     * @param r row of target
     * @param c column of target
     * @param puzz char array with the puzzle
     * @return manhattan distance
     */
    private static int manhattan(int r, int c, int[][]puzz) {
        int t = puzz[r][c] - 65;
        int t_row = t / 4; int t_col = t % 4;
        return (t_row - r + t_col - t >= 0) ? (t_row - r + t_col - t) : -(t_row - r + t_col - t);
    }

    /**
     * Returns the total cost at some state
     * @param puzz puzzle to calculate
     * @return total cost
     */
    private static int tot_cost(int[][] puzz) {
        int t = 0;
        for(int i = 0; i < 4; i++)
            for(int it = 0; it < 4; it++)
                t += manhattan(i, it, puzz);
        return t;
    }

    /**
     * this works but I have no idea why
     * @param puzz the puzzle
     */
    private static void better_dijkstra(int[][] puzz) {
        int t = tot_cost(puzz);
        for(int i = 0; i < 4; i++)
            for(int it = 0; it < 4; it++)
                if(puzz[i][it] = -1)
    }
}
