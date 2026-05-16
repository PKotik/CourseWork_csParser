public class MyClass {
    int x;
    private int y;
    static int z;

    int Method1() {
        int a = 5;
        int b;
        a = 10;
        if (a >= b) {
            return a;
        } else {
            return b;
        }
    }

    public void Method2() {
        int x = 0;
        while (x <= 10) {
            x = x + 1;
        }
        return;
    }

    public void Method3() {
        return;
    }

    private int Method4() {
        return 3;
    }
}
