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

    void Method3(DocumentModelEditDto editModel) 
    {
        var hideFieldKeys = GetFieldsKeysForHideViewModel();
        foreach (var hideField in hideFieldKeys) {}
    }

    private int Method4() {
        return 3;
    }

    public void Method5() {
        Console.WriteLine(123);
    }
}
