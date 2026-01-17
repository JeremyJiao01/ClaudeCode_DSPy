           checkBitcastSrcVectorSize(Src.getOperand(1), Size, AllowTruncate) &&
           checkBitcastSrcVectorSize(Src.getOperand(2), Size, AllowTruncate);
  case ISD::BUILD_VECTOR:
    return ISD::isBuildVectorAllZeros(Src.getNode()) ||
           ISD::isBuildVectorAllOnes(Src.getNode());
  }
  return false;
}