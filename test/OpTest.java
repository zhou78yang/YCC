public class OpTest
{
    public static void main(String [] args)
    {
        int a, b = a, c = b, d = c;
        b = a * b + c * d;
        c = a * b / c + d;
        d = a * (b + c) / d;
        static int count;
        int [] arr = { 1, 2, 3, 4 }, arr2 = arr[0], arr3 = {};
        if(a == b && a == c || b == c && c == d)
        {
            print("这尼玛什么鬼");
        }
        else
        {
            print("hello world");
        }
    }
}
