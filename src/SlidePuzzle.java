import java.io.*;
import java.util.Scanner;
import java.util.PriorityQueue;

public class SlidePuzzle {
    private static char[][] puzzle = new char[4][4];
    public static void main(String[] args) throws FileNotFoundException {
        Scanner s = new Scanner(new BufferedReader(new FileReader("puzzle.txt")));
        for(int i = 0; i < 4; i++)
            for(int it = 0; it < 4; it++)
                puzzle[i][it] = s.next().charAt(0);
        System.out.println("Puzzle read!");
    }

}
