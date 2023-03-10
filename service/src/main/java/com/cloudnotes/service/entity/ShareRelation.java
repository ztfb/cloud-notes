package com.cloudnotes.service.entity;

import lombok.Data;

import javax.persistence.*;

@Entity
@Data
@Table(name = "share_relation")
public class ShareRelation {
    @Id
    @GeneratedValue(strategy = GenerationType.IDENTITY)
    private int id;
    @Column(name = "note_id")
    private int noteId;
    @Column(name = "user_id")
    private int userId;
    @Column(name = "share_id")
    private int shareId;
    @Column(name = "power")
    private int power;
    @Column(name = "note_name")
    private String noteName;
}
