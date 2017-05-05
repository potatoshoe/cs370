#! /bin/bash

source "$(dirname $0)/config.sh"

fn_main () {

    if [ "$(whoami)" != "root" ]; then
        die "This script must be run as root."
    fi


    fn_install_packages
    fn_setup_user
    fn_setup_ssh
    fn_setup_hosts
    fn_setup_nfs_server
    fn_setup_nodes_file

}

die () {
    #statements
    echo "ERROR: $1"
    exit 1
}

fn_setup_user () {

    echo "Creating user '$MPIUSER_USERNAME' with default password '$MPIUSER_PASSWORD'..."

    adduser --disabled-password --gecos "" $MPIUSER_USERNAME
    echo $MPIUSER_USERNAME:$MPIUSER_PASSWORD | chpasswd

    echo "Done."

}

fn_setup_hosts () {

    echo "Configuring hostnames..."

    hostname $MASTER_HOST
    echo "$MASTER_HOST" > /etc/hostname

    cat <<EOF > /etc/network/interfaces
source-directory /etc/network/interfaces.d

auto lo
iface lo inet loopback

auto eth0
iface eth0 inet static
    address $MASTER_IP
    netmask $SUBNET_MASK
    gateway $GATEWAY_IP
    nameserver $DNS_SERVERS

allow-hotplug wlan0
iface wlan0 inet manual
    wpa-conf /etc/wpa_supplicant/wpa_supplicant.conf

allow-hotplug wlan1
iface wlan1 inet manual
    wpa-conf /etc/wpa_supplicant/wpa_supplicant.conf
EOF

    cat <<EOF > /etc/hosts
127.0.0.1   localhost
127.0.1.1   $MASTER_HOST

$MASTER_IP  $MASTER_HOST
$SLAVE1_IP  $SLAVE1_HOST
$SLAVE2_IP  $SLAVE2_HOST
EOF

    systemctl restart networking

    echo "Done."

}

fn_install_packages () {

    echo "Installing required packages..."

    apt-get install -y \
    mpich2 \
    libmpich2-dev \
    openssh-server \
    openssh-client \
    nfs-kernel-server \
    vim \
    zsh

    echo "Done."
}

fn_setup_nfs_server () {

    echo "Setting up NFS server and shared folder..."

    if [ ! -d "$SHARED_DIR_PATH" ]; then
        mkdir $SHARED_DIR_PATH
        chown $MPIUSER_USERNAME:$MPIUSER_USERNAME $SHARED_DIR_PATH
        chmod 770 $SHARED_DIR_PATH
    fi
    echo "$SHARED_DIR_PATH *(rw,sync,no_root_squash,no_subtree_check)" > /etc/exports
    exportfs -a
    systemctl restart nfs-kernel-server

    echo "Done."

}

fn_setup_ssh () {

    echo "Creating SSH public and private keys..."

    if [ ! -d "$MPIUSER_HOME/.ssh" ]; then
        mkdir $MPIUSER_HOME/.ssh
        chown $MPIUSER_USERNAME:$MPIUSER_USERNAME $MPIUSER_HOME/.ssh
        chmod 700 $MPIUSER_HOME/.ssh
    fi

    if [ ! -e "$MPIUSER_HOME/.ssh/authorized_keys" ]; then
        touch $MPIUSER_HOME/.ssh/authorized_keys
        chown $MPIUSER_USERNAME:$MPIUSER_USERNAME $MPIUSER_HOME/.ssh/authorized_keys
        chmod 644 $MPIUSER_HOME/.ssh/authorized_keys
    fi

    cat $SCRIPT_DIR/id_rsa.pub > $MPIUSER_HOME/.ssh/authorized_keys
    cp $SCRIPT_DIR/id_rsa $MPIUSER_HOME/.ssh/id_rsa_master

    cat <<EOF > $MPIUSER_HOME/.ssh/config
Host $SLAVE1_HOST
    IdentitiesOnly yes
    IdentityFile ~/.ssh/id_rsa_master
    User $MPIUSER_USERNAME
    StrictHostKeyChecking no
    LogLevel ERROR

Host $SLAVE2_HOST
    IdentitiesOnly yes
    IdentityFile ~/.ssh/id_rsa_master
    User $MPIUSER_USERNAME
    StrictHostKeyChecking no
    LogLevel ERROR
EOF

    echo "Done. SSH public key is located at '$MPIUSER_HOME/.ssh/id_rsa_master.pub'. Please distribute to slave nodes."
}

fn_setup_nodes_file () {

    echo "Setting up nodes file for 'mpiexec' command."

    cat <<EOF > $SHARED_DIR_PATH/nodes
$MASTER_HOST
$SLAVE1_HOST
$SLAVE2_HOST
EOF
    chown $MPIUSER_USERNAME:$MPIUSER_USERNAME $SHARED_DIR_PATH/nodes
    chmod 664 $SHARED_DIR_PATH/nodes


    echo "Done."

}

fn_main
