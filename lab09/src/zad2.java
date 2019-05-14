import java.util.Random;
import java.util.concurrent.locks.ReentrantLock;

class threadZad2 extends Thread {

    private int number;

    private int sizeX;

    private int sizeY;

    private char c;

    private char[][] image;

    threadZad2(
            int number,
            char c,
            char[][] image,
            int sizeX,
            int sizeY
    ) {
        this.number = number;
        this.c = c;
        this.image = image;
        this.sizeX = sizeX;
        this.sizeY = sizeY;
    }

    public void run() {
        zad2.lock.lock();
        try {
            System.out.print("Watek numer " + number + ": " + c + " ");
            for (int i = 0; i < sizeY; i++) {
                for (int j = 0; j < sizeX; j++) {
                    if (image[i][j] == c) {
                        System.out.print("=");
                    }
                }
            }

            System.out.println();
        } finally {
            zad2.lock.unlock();
        }
    }
}

public class zad2 {

    static final ReentrantLock lock = new ReentrantLock();

    private static final Random random = new Random();

    private static final int m = 10, n = 12;

    public static void main(String[] args) {
        func();
    }

    private static void func() {
        int numThreads = 10;
        char c = '&';

        char[][] image = new char[m][n];
        System.out.println("Obraz:");

        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                image[i][j] = (char) (c + random.nextInt(numThreads));
                System.out.print(image[i][j]);
            }
            System.out.println();
        }

        System.out.println();

        threadZad2[] newThreadZad2 = new threadZad2[numThreads];

        for (int i = 0; i < numThreads; i++) {
            (newThreadZad2[i] = new threadZad2(i, c++, image, n, m)).start();
        }

        for (int i = 0; i < numThreads; i++) {
            try {
                newThreadZad2[i].join();
            } catch (InterruptedException e) {
                System.out.println(e.getMessage());
            }
        }
    }
}
