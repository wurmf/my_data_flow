package example1;

public class MyKernel extends Kernel {
    private static final DFEType type = dfeInt(32);

    public MyKernel(KernelParameters parameters){
        super(parameters);

        DFEVar x = io.input("x",type);
        DFEVar y = io.input("y",type);
        DFEVar z = io.input("z",type);

        //Should be possible with control.mux()
        //This will probably not work
        DFEVar output = z == x ? x * y * z : ;
        output = z < x ? (x + z) * 2 : ;
        output = z > x ? (x + y) + 2 : ;

        io.output("result",output,type);
    }
}




