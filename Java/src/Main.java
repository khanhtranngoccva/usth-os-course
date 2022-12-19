public class Main {
    int data = 0;

    public synchronized void get() {
        if (data <= 0) {
            try {
                wait();
            } catch (Exception ignored) {
                System.out.println("System error.");
            }
        }
        data--;
    }

    public synchronized void set() {
        data++;
        System.out.println(data);
        this.notify();
    }

    public static void main(String[] args) {
        Main primaryObj = new Main();
        while (true) {
            primaryObj.set();
            primaryObj.get();
        }
    }
}