import scala.collection.mutable.ListBuffer
import scala.io.Source
import scala.util.Using

def solve(xs: List[Int]): Int = {
    var count = 0
    var prev = Int.MaxValue
    for (i <- 0 until xs.length - 2) {
        val sum = xs(i) + xs(i+1) + xs(i+2)
        if (prev < sum) {
            count += 1
        }
        prev = sum
    }
    count
}

@main
def main = {
    var xs = ListBuffer[Int]()
    for (line <- Source.stdin.getLines) {
        xs += line.toInt
    }
    println(solve(xs.toList))
}
