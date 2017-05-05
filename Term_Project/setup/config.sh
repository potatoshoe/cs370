export SCRIPT_DIR="$(dirname $0)"

export MPIUSER_USERNAME="mpiuser"
export MPIUSER_PASSWORD="mpiuser"
export MPIUSER_HOME="/home/$MPIUSER_USERNAME"

export SHARED_DIR_PATH="$MPIUSER_HOME/mpi-share"

export GATEWAY_IP="10.0.0.1"
export SUBNET_MASK="255.255.255.0"
export BROADCAST_ADDR="10.0.0.255"
export DNS_SERVERS="8.8.8.8 8.8.4.4"

export MASTER_HOST="master"
export MASTER_IP="10.0.0.100"

export SLAVE1_HOST="slave1"
export SLAVE1_IP="10.0.0.101"

export SLAVE2_HOST="slave2"
export SLAVE2_IP="10.0.0.102"
